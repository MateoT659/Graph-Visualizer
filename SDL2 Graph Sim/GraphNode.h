#pragma once
#include"GraphSimConst.h"

class GraphNode
{
private:
	int x, y, radius;
	SDL_Color color;

public:
	GraphNode(int, int, int, SDL_Color);

	void setPos(int, int);

	void setColor(SDL_Color);

	void render();

	void renderSkeleton();

	bool cotainsPoint(int, int);

	int getX();

	int* getXaddr();

	int getY();

	int* getYaddr();

	int getRadius();
};
