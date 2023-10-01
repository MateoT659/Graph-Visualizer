#pragma once
#include"GraphSimConst.h"



class GraphNode
{
private:
	Vec2 pos;
	int radius;
	SDL_Color color;
	NodeType type;
	bool ghost;

	void renderFilled();
	void renderSkeleton();

public:
	GraphNode(int x, int y, int radius, SDL_Color color, NodeType type);
	GraphNode(Vec2 pos, int radius, SDL_Color color, NodeType type);

	GraphNode* copy();

	void setPos(int x, int y);
	void setPos(Vec2 position);

	Vec2 getPos();
	int getX();
	int* getXaddr();
	int getY();
	int* getYaddr();

	int getRadius();
	void setRadius(int radius);

	void setColor(SDL_Color color);

	void toggleGhost();

	void render();

	bool containsPoint(Vec2 position);
	bool containsPoint(int x, int y);
};
