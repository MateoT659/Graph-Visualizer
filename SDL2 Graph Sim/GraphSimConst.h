#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
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
class Textbox;
class NodeText;
class Gate;
class UnaryGate;
class BinaryGate;

typedef enum NodeType {
	Filled, Skeleton, Cross, FilledSq, OpenSq, CrossSq, GATE
} NodeType;
const int nodeTypeTotal = 6;

typedef enum EdgeType {
	None, Directed, Resistance, Dashed, Dotted, Switch, VertFirst, HorFirst 
} EdgeType;
const int edgeTypeTotal = 8;

typedef enum uGateType {
	IDENTITY, NOT
} uGateType;
const int uGateTypeTotal = 2;

typedef enum bGateType {
	AND = 2, NAND = 3, OR = 4, NOR = 5, XOR = 6, XNOR = 7
} bGateType;
const int bGateTypeTotal = 6;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
const double sqrt2 = sqrt(2);
const double sqrt34 = sqrt(34);
const double sqrt61 = sqrt(61);

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
extern SDL_Color bgColor;
extern EdgeType edgeType;
extern std::string currentToolTip;

extern SDL_Window* window;
extern SDL_Surface* windowSurface;
extern SDL_Renderer* renderer;
extern TTF_Font* font;

extern GraphNode* ghost;
extern Gate* ghostGate;
extern std::vector<GraphNode*> nodes;
extern std::vector<GraphEdge*> edges;
extern std::unordered_set<GraphEdge*> switches;
extern std::vector<Textbox*> textboxes;
extern std::vector<NodeText*> nodetexts;
extern std::vector<FreeEdge*> freeEdges;
extern std::unordered_set<FreeEdge*> fswitches;
extern std::vector<Gate*> gates;
extern Textbox* tip;
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
extern std::vector<Icon*> textIcons;
extern std::vector<Icon*> colorIcons;
extern std::vector<Icon*> editIcons;
extern int selectedEditTool;
extern int selectedColorTool;
extern int selectedTextTool;
extern std::string currentFilepath;
extern std::vector<Icon*>gateIcons;

//main.cpp
void initSDL(void);
void initTextures(void);
void renderToolTip();

//input.cpp
void parseEvent(SDL_Event* event);
Vec2 getMousePos();
bool rectIsTouched(SDL_Rect rect, int x, int y);
bool rectIsTouched(SDL_Rect rect, Vec2 pos);
bool lineIntersectsLine(Vec2 v1, Vec2 v2, Vec2 u1, Vec2 u2);
bool lineIntersectsRect(SDL_Rect rect, Vec2 p1, Vec2 p2);

//graphics.cpp
void updateUIOnMove();
void drawCircle(Vec2 pos, int radius);
void drawCircle(int cx, int cy, int radius);
void drawOpenCircle(Vec2 pos, int radius);
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
void drawOpenRectangle(int x, int y, int w, int h, SDL_Color color);
void drawOpenRectangle(SDL_Rect rect, SDL_Color color);
void drawOpenRectangle(int x, int y, int w, int h);
void drawOpenRectangle(SDL_Rect rect);
void render(bool showGhost);
void renderObjects(bool showGhost);
void renderInterface();
void renderU(bool showGhost);
void updateIcons();
SDL_Texture* loadTexture(std::string filepath);
bool colorEquals(SDL_Color color1, SDL_Color color2);

//file_dialog.cpp
void openColorPicker();
void initFiles(void);
void openFile();
void saveAsFile();
void saveFile();
void adjustCustomColors();