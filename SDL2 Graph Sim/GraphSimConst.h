#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <windows.h>
#include <sstream>
#include <fstream>
#include "Vec2.h"

class GraphNode;
class GraphEdge;
class FreeEdge;
class Image;
class Icon;


typedef enum NodeType {
	Filled, Skeleton, Cross, FilledSq, OpenSq, CrossSq
} NodeType;
const int nodeTypeTotal = 6;

typedef enum EdgeType {
	None, Directed, Resistance, Dashed, Dotted, Switch
} EdgeType;
const int edgeTypeTotal = 6;

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 1000;
const double sqrt2 = sqrt(2);

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
extern SDL_Surface* windowSurface;
extern SDL_Renderer* renderer;

extern GraphNode* ghost;
extern std::vector<GraphNode*> nodes;
extern std::vector<GraphEdge*> edges;
extern std::unordered_set<GraphEdge*> switches;
extern std::vector<FreeEdge*> freeEdges;
extern std::unordered_set<FreeEdge*> fswitches;

extern bool running;

extern OPENFILENAMEA ofn;       // common dialog box structure
extern char szFile[260];       // buffer for file name
extern HWND hwnd;              // owner window
extern CHOOSECOLOR cc;                 // common dialog box structure 
extern COLORREF acrCustClr[16]; // array of custom colors 
extern HBRUSH hbrush;           // brush handle
extern DWORD rgbCurrent;        // initial color selection

extern std::vector<Icon*> icons;
extern int selectedInd;
extern Image* sidebar;
extern SDL_Texture* hoveredTexture;
extern SDL_Texture* selectedTexture;
extern std::vector<Icon*> nodeIcons;
extern std::vector<Icon*> edgeIcons;

//main.cpp
void initSDL(void);
void initTextures(void);

//input.cpp
void parseEvent(SDL_Event* event);
Vec2 getMousePos();
bool rectIsTouched(SDL_Rect rect, int x, int y);
bool rectIsTouched(SDL_Rect rect, Vec2 pos);
bool lineIntersectsLine(Vec2 v1, Vec2 v2, Vec2 u1, Vec2 u2);
bool lineIntersectsRect(SDL_Rect rect, Vec2 p1, Vec2 p2);

//graphics.cpp
void drawCircle(Vec2 pos, int radius);
void drawCircle(int cx, int cy, int radius);
void setRenderColor(SDL_Color color);
void setRenderColor(SDL_Color rgbColor, int alpha);
void setRenderColor(int r, int g, int b, int alpha);
void drawPoint(int x, int y);
void drawPoint(Vec2 pos);
void drawLine(int x1, int y1, int x2, int y2);
void drawLine(Vec2 point1, Vec2 point2);
void clearScreen(SDL_Color color);
void drawFilledRectangle(int x, int y, int w, int h, SDL_Color color);
void drawFilledRectangle(SDL_Rect rect, SDL_Color color);
void drawFilledRectangle(int x, int y, int w, int h);
void drawFilledRectangle(SDL_Rect rect);
void render(bool showGhost);
void renderObjects(bool showGhost);
void renderInterface();
void renderU(bool showGhost);
void updateIcons();
SDL_Texture* loadTexture(std::string filepath);

//file_dialog.cpp
void openColorPicker();
void initFiles(void);
void openFile();
void saveAsFile();
void saveFile();
void adjustCustomColors();