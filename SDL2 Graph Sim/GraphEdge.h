#pragma once
#include "GraphSimConst.h"
#include "GraphNode.h"

class GraphEdge
{
private:
	GraphNode* node1;
	GraphNode* node2;
	SDL_Color color;

	bool directed;
	double slope;
	int b, ymin, ymax, xmin, xmax;

public:
	GraphEdge(GraphNode*, GraphNode*, SDL_Color, bool);
	GraphNode* getNode1();
	GraphNode* getNode2();
	SDL_Color getColor();
	void setColor(SDL_Color color);
	
	void update();
	bool hasNode(GraphNode*);
	void render(void);
	void renderDi();
	void renderUnDi();
	bool isTouched(int, int);
};

