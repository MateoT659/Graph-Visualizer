#include"GraphSimConst.h"
#include"GraphEdge.h"
#include"GraphNode.h"

//save and loading .gphv files (text files with info about layout)

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
	std::ifstream inStream;
	std::string filename, line, word;
	filename = getOpenPath();
	if (filename == "ERROR") {
		return;
	}
	inStream.open(filename);
	inStream >> word;
	currentColor.r = stoi(word);
	inStream >> word;
	currentColor.g = stoi(word);
	inStream >> word;
	currentColor.b = stoi(word);

	inStream.close();
	
	
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

	outStream << std::to_string(currentColor.r) << " " << std::to_string(currentColor.g) << " " << std::to_string(currentColor.b);

	outStream.close();
}