#pragma once
#include "GraphSimConst.h"
#include "GraphNode.h"

class GraphEdge
{
private:
	GraphNode* node1;
	GraphNode* node2;
	SDL_Color color;
	EdgeType type;
	double unitX, unitY;
	

	void renderDirected();
	void renderNone();
	
	double slope;
	int b, ymin, ymax, xmin, xmax;

public:
	GraphEdge(GraphNode* node1, GraphNode* node2, SDL_Color color, EdgeType type);
	GraphNode* getNode1();
	GraphNode* getNode2();
	SDL_Color getColor();
	void setColor(SDL_Color color);

	EdgeType getType();
	
	void update();
	bool containsNode(GraphNode* node);
	void render(void);
	bool isTouched(int x, int y);
	bool isTouched(Vec2 pos);
};

