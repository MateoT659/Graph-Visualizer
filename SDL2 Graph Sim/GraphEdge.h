#pragma once
#include "GraphSimConst.h"
#include "GraphNode.h"

class GraphEdge
{
private:
	GraphNode* node1;
	GraphNode* node2;
	SDL_Color color;

	double slope;
	int b, ymin, ymax, xmin, xmax;

public:
	GraphEdge(GraphNode*, GraphNode*, SDL_Color);
	GraphNode* getNode1();
	GraphNode* getNode2();
	void setColor(SDL_Color color);
	
	void update();
	bool hasNode(GraphNode*);
	void render(void);
	bool isTouched(int, int);
};

