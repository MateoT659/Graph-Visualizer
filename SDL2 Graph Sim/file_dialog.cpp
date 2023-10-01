#include"GraphSimConst.h"
#include"GraphEdge.h"
#include"GraphNode.h"

//save and loading .gphv files (text files with info about layout)
bool error;

char hexMap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

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
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "GraphV File\0 * .gphv\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrDefExt = "gphv";
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;



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
	filename = getOpenPath();
	if (filename == "ERROR") {
		return;
	}
	inStream.open(filename);
	inStream >> word;
	
	SDL_Color fileColor;
	EdgeType fileEdgeType;
	GraphNode* fileGhost;
	std::vector<GraphNode*> fileNodes;
	std::vector<GraphEdge*> fileEdges;
	

	fileColor = hexToColor(word);
	if (error) return;

	inStream.close();
	
	currentColor = fileColor;
	
}

void saveFile() {
	//use ofstream to save current status to file
	std::ofstream outStream;
	std::string filename, line, word;
	filename = getSavePath();
	if (filename == "ERROR"){
		currentColor = BLACK;
		return;
	}
	outStream.open(filename);
	
	outStream << toHex(currentColor) << " ";

	outStream.close();
}