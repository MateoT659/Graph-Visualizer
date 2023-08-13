#pragma once
#include <SDL.h>
#include <iostream>
#include <stdio.h>
#include <vector>

class GraphNode;
class GraphEdge;
class GraphDirectedEdge;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

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

extern SDL_Color currentColor;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern std::vector<GraphNode*> nodes;

extern std::vector<GraphEdge*> edges;

extern bool running;
extern bool diGraph;

void initSDL(void);
void clearScreen(SDL_Color);
void parseEvent(SDL_Event*);
void drawFilledRectangle(SDL_Rect);
void render();