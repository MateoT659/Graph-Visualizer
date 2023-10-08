#include "Includes.h"

bool error;


OPENFILENAMEA ofn;       // common dialog box structure
char szFile[260];       // buffer for file name
HWND hwnd = { 0 };              // owner window
CHOOSECOLOR cc;                 // common dialog box structure 
COLORREF acrCustClr[16]; // array of custom colors 
HBRUSH hbrush;       // brush handle
DWORD rgbCurrent;        // initial color selection
std::string currentFilepath;

char hexMap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

int colorToInt(SDL_Color c) {
	int i = 0;
	i += c.r;
	i += c.g * 256;
	i += c.b * 256 * 256;
	return i;
}

int hexCharHelper(char c) {
    if ('0' <= c && c <= '9') {
         return (int)c - 48;
	}
	else if ('A' <= c && c <= 'F') {
         return (int)c - 55;
	}
    return -1;
}
std::string toHex(SDL_Color color) {
	//return "ERROR" if error
	std::string ret = "#";
	
	ret += hexMap[(color.r / 16) % 16];
	ret += hexMap[color.r % 16];

	ret += hexMap[(color.g / 16) % 16];
	ret += hexMap[color.g % 16];

	ret += hexMap[(color.b / 16) % 16];
	ret += hexMap[color.b % 16];
	
	return ret;
}
SDL_Color hexToColor(std::string hex) {
	//return NULL if error

	if (hex[0] != '#') {
		error = true;
		return { 0 };
	}
	SDL_Color ret = { 0, 0, 0, 255 };
	
	int num1 = hexCharHelper(hex[1]);
	int num2 = hexCharHelper(hex[2]);
	if (num1 == -1 || num2 == -1) {
		error = true;
		return { 0 };
	}
	ret.r = (Uint8)(num1*16 + num2);

	num1 = hexCharHelper(hex[3]);
	num2 = hexCharHelper(hex[4]);
	if (num1 == -1 || num2 == -1) {
		error = true;
		return { 0 };
	}
	ret.g = (Uint8)(num1 * 16 + num2);

	num1 = hexCharHelper(hex[5]);
	num2 = hexCharHelper(hex[6]);
	if (num1 == -1 || num2 == -1) {
		error = true;
		return { 0 };
	}
	ret.b = (Uint8)(num1 * 16 + num2);

	return ret;
}

void initFiles() {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "GraphV File\0 * .gphv\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrDefExt = "gphv";
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	currentFilepath = "None";
	
	acrCustClr[0] = colorToInt(WHITE);
	acrCustClr[1] = colorToInt(PINK);
	acrCustClr[2] = colorToInt(RED);
	acrCustClr[3] = 255;
	acrCustClr[4] = colorToInt(ORANGE);
	acrCustClr[5] = colorToInt({ 248,163,141, 255 });
	acrCustClr[6] = colorToInt(YELLOW);
	acrCustClr[7] = colorToInt(GREEN);
	acrCustClr[8] = colorToInt({ 1, 102, 67, 255 });
	acrCustClr[9] = colorToInt(CYAN);
	acrCustClr[10] = colorToInt(BLUE);
	acrCustClr[11] = colorToInt(PURPLE);
	acrCustClr[12] = colorToInt(AMETHYST);
	acrCustClr[13] = colorToInt({135, 175, 115, 255});
	acrCustClr[14] = colorToInt({ 116,71,0, 255 });
	acrCustClr[15] = colorToInt({ 151, 151,151, 255 });
	

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
}

void openColorPicker() {
	if (ChooseColor(&cc) == TRUE)
	{
		currentColor.r = cc.rgbResult % 256;
		currentColor.g = (cc.rgbResult / 256) % 256;
		currentColor.b = (cc.rgbResult / (256 * 256)) % 256;
		ghost->setColor(currentColor);
		adjustCustomColors();
	}
}
void adjustCustomColors() {
	int col = colorToInt(currentColor);
	if (col != acrCustClr[0]) {
		for (int i = 14; i >= 0; i--) {
			acrCustClr[i + 1] = acrCustClr[i];
		}
		acrCustClr[0] = colorToInt(currentColor);
		cc.lpCustColors = acrCustClr;
	}
}

std::string getOpenPath() {
	// Display the Open dialog box. 
	std::string filename = "ERROR";

	if (GetOpenFileNameA(&ofn)) {
		filename = ofn.lpstrFile;
	}

	return filename;
}
std::string getSavePath() {
	std::string filename = "ERROR";
	if(GetSaveFileNameA(&ofn)){
		filename = ofn.lpstrFile;
	}
	return filename;
}

void openFile() {
	//use ifstream to upload status from file
	error = false;
	
	std::ifstream inStream;
	std::string filename, line, word;
	int num;

	filename = getOpenPath();
	if (filename == "ERROR") {
		return;
	}

	SDL_Color fileColor;
	EdgeType fileEdgeType;
	GraphNode* fileGhost;
	std::vector<GraphNode*> fileNodes;
	std::vector<GraphEdge*> fileEdges;

	inStream.open(filename);
	std::getline(inStream, line);

	std::vector<std::string> wordVec;
	std::stringstream strStream(line);
	while (strStream >> word) wordVec.push_back(word);

	if (wordVec.size() != 3) return;
	//currentColor
	fileColor = hexToColor(wordVec[0]);
	//edgeType
	try {
		num = stoi(wordVec[1]);
	}
	catch (...) {
		return;
	}
	if (num < 0 || num>=edgeTypeTotal) return;
	fileEdgeType = (EdgeType)num;
	//ghost
	try {
		num = stoi(wordVec[2]);
	}
	catch (...) {
		return;
	}
	if (num < 0 || num >= nodeTypeTotal) return;
	fileGhost = new GraphNode(0, 0, 15, fileColor, (NodeType)num);
	fileGhost->toggleGhost();
	wordVec.clear();

	//nodes
	int x, y, radius, type;
	while (!error && getline(inStream, line) && line.size() != 0) {
		std::stringstream strStream(line);
		
		while (strStream >> word) wordVec.push_back(word);

		if (wordVec.size() != 5){
			return;
		}
		
		try {
			x = stoi(wordVec[0]);
			y = stoi(wordVec[1]);
			radius = stoi(wordVec[2]);
			type = stoi(wordVec[4]);
		}
		catch (...) {
			return;
		}
		if (x<0 || x>=SCREEN_WIDTH) return;
		if (y<0 || y>=SCREEN_HEIGHT) return;
		if (type<0 || type>=nodeTypeTotal) return;

		fileNodes.push_back(new GraphNode(x, y, radius, hexToColor(wordVec[3]), (NodeType)type));

		wordVec.clear();
	}

	//edges
	int ind1, ind2, etype;
	while (!error && getline(inStream, line)) {
		std::stringstream strStream(line);

		while (strStream >> word) wordVec.push_back(word);

		if (wordVec.size() != 4) return;

		try {
			ind1 = stoi(wordVec[0]);
			ind2 = stoi(wordVec[1]);
			etype = stoi(wordVec[3]);
		}
		catch (...) {
			return;
		}
		if (ind1 < 0 || ind1 >= fileNodes.size()) return;
		if (ind2 < 0 || ind2 >= fileNodes.size()) return;
		if (etype < 0 || etype >= edgeTypeTotal) return;

		fileEdges.push_back(new GraphEdge(fileNodes[ind1], fileNodes[ind2], hexToColor(wordVec[2]), (EdgeType)etype));

		wordVec.clear();
	}

	inStream.close();
	if (error) return;

	currentColor = fileColor;
	edgeType = fileEdgeType;
	delete ghost;
	ghost = fileGhost;
	for (GraphNode *node : nodes) {
		delete node;
	}
	nodes.clear();
	for (GraphEdge* edge : edges) {
		delete edge;
	}
	edges.clear();

	nodes = fileNodes;
	edges = fileEdges;
	currentFilepath = filename;

	updateIcons();
}
void saveAsFile() {
	//use ofstream to save current status to file
	std::ofstream outStream;
	std::string filename, line, word;
	filename = getSavePath();
	if (filename == "ERROR") {
		return;
	}
	outStream.open(filename);

	//currentColor
	outStream << toHex(currentColor) << " ";

	//edgeType
	outStream << (int)edgeType << " ";

	//ghost nodeType
	outStream << (int)(ghost->getType()) << "\n";

	std::unordered_map<GraphNode*, int> indexMap;
	//input all nodes on seperate lines: x, y, radius, color, type, 
	for (int i = 0; i < nodes.size(); i++) {
		outStream << nodes[i]->getX() << " "
			<< nodes[i]->getY() << " "
			<< nodes[i]->getRadius() << " "
			<< toHex(nodes[i]->getColor()) << " "
			<< nodes[i]->getType() << "\n";
		indexMap[nodes[i]] = i;
	}

	outStream << "\n";

	//input all edges on seperate lines: ind1, ind2, color, type

	for (int i = 0; i < edges.size(); i++) {
		outStream << indexMap[edges[i]->getNode1()] << " "
			<< indexMap[edges[i]->getNode2()] << " "
			<< toHex(edges[i]->getColor()) << " "
			<< (int)edges[i]->getType() << "\n";
	}

	currentFilepath = filename;


	outStream.close();
}
void saveFile() {
	//use ofstream to save current status to file
	
	std::ofstream outStream;
	std::string filename, line, word;
	if (currentFilepath == "None") {
		filename = getSavePath();
		if (filename == "ERROR") {
			return;
		}
	}
	else {
		filename = currentFilepath;
	}
	outStream.open(filename);
	
	//currentColor
	outStream << toHex(currentColor) << " ";

	//edgeType
	outStream << (int)edgeType << " ";

	//ghost nodeType
	outStream << (int)(ghost->getType()) << "\n";

	std::unordered_map<GraphNode*,int> indexMap;
	//input all nodes on seperate lines: x, y, radius, color, type, 
	for (int i = 0; i < nodes.size(); i++) {
		outStream << nodes[i]->getX() << " "
			<< nodes[i]->getY() << " "
			<< nodes[i]->getRadius() << " "
			<< toHex(nodes[i]->getColor()) << " "
			<< nodes[i]->getType() << "\n";
		indexMap[nodes[i]] = i;
	}

	outStream << "\n";

	//input all edges on seperate lines: ind1, ind2, color, type

	for (int i = 0; i < edges.size(); i++) {
		outStream << indexMap[edges[i]->getNode1()] << " "
			<< indexMap[edges[i]->getNode2()] << " "
			<< toHex(edges[i]->getColor()) << " "
			<< (int)edges[i]->getType() << "\n";
	}

	currentFilepath = filename;

	outStream.close();
}