#pragma once
#include <SDL.h>
#include <iostream>
#include <stdio.h>
#include <vector>

class GraphNode;
class GraphEdge;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const SDL_Color WHITE = {255, 255, 255, 255};
const SDL_Color BLACK = {0, 0, 0, 255};
const SDL_Color BLUE = {0, 128, 255, 255 };
const SDL_Color RED = { 255,0,127,255 };
const SDL_Color GREEN = { 0, 204, 102 ,255};
const SDL_Color PURPLE = { 102,0,204,255 };

extern SDL_Color currentColor;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern std::vector<GraphNode*> nodes;

extern std::vector<GraphEdge*> edges;

extern bool running;

void initSDL(void);
void clearScreen(SDL_Color);
void parseEvent(SDL_Event*);
void drawFilledRectangle(SDL_Rect);
void render();