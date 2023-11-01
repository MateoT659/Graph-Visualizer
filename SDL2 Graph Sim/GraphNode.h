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
	void renderCrossed();
	void renderFilledSq();
	void renderSkeletonSq();
	void renderCrossedSq();

public:
	GraphNode(int x, int y, int radius, SDL_Color color, NodeType type);
	GraphNode(Vec2 pos, int radius, SDL_Color color, NodeType type);

	GraphNode* copy();

	void setPos(int x, int y);
	void setPos(Vec2 position);

	void translateBy(Vec2 vec);

	Vec2 getPos();
	int getX();
	int* getXaddr();
	int getY();
	int* getYaddr();

	int getRadius();
	void setRadius(int radius);

	SDL_Color getColor();
	void setColor(SDL_Color color);

	NodeType getType();
	void setType(NodeType type);

	void toggleGhost();

	void render();

	bool containsPoint(Vec2 position);
	bool containsPoint(int x, int y);
};
