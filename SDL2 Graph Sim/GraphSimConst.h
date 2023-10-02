#pragma once
#include <SDL.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <sstream>
#include <fstream>
#include "Vec2.h"

class GraphNode;
class GraphEdge;

typedef enum EdgeType {
	None, Directed
} EdgeType;
const int edgeTypeTotal = 2;


typedef enum NodeType {
	Filled, Skeleton
} NodeType;
const int nodeTypeTotal = 2;

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 1000;

const SDL_Color WHITE = {255, 255, 255, 255};
const SDL_Color BLACK = {0, 0, 0, 255};
const SDL_Color RED = { 187,0,0 ,255 };
const SDL_Color AMETHYST = { 144,99,205,255 };
const SDL_Color BLUE = { 8,39,245, 255 };
const SDL_Color CYAN = { 0,188,227 , 255 };
const SDL_Color PINK = { 255,0,127,255 };
const SDL_Color GREEN = { 50,245,50 ,255 };
const SDL_Color PURPLE = { 102,0,204,255 };
const SDL_Color YELLOW = {255, 234, 0, 255};
const SDL_Color ORANGE = { 255,133,15 ,255 };

extern SDL_Rect colorBox;
extern SDL_Color currentColor;
extern EdgeType edgeType;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern GraphNode* ghost;
extern std::vector<GraphNode*> nodes;
extern std::vector<GraphEdge*> edges;

extern bool running;

extern OPENFILENAMEA ofn;       // common dialog box structure
extern char szFile[260];       // buffer for file name
extern HWND hwnd;              // owner window

//main.cpp
void initSDL(void);

//input.cpp
void parseEvent(SDL_Event* event);
Vec2 getMousePos();

//graphics.cpp
void setRenderColor(SDL_Color color);
void setRenderColor(SDL_Color rgbColor, int alpha);
void setRenderColor(int r, int g, int b, int alpha);
void drawPoint(int x, int y);
void drawPoint(Vec2 pos);
void drawLine(int x1, int y1, int x2, int y2);
void drawLine(Vec2 point1, Vec2 point2);
void clearScreen(SDL_Color color);
void drawFilledRectangle(SDL_Rect rect);
void render(bool);

//file_dialog.cpp
void initFiles(void);
std::string getOpenPath();
void openFile();
std::string getSavePath();
void saveFile();
