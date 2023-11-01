#pragma once
#include "GraphSimConst.h"
#include "GraphNode.h"
#include "Gate.h"

class GraphEdge
{
private:
	GraphNode* node1;
	GraphNode* node2;
	SDL_Color color;
	EdgeType type;
	double unitX, unitY;
	
	void renderHorL();
	void renderVertL();
	void renderSwitch();
	void renderDotted();
	void renderDashed();
	void renderResist();
	void renderDirected();
	void renderNone();
	
	Vec2 to, from;
	double slope;
	int b, ymin, ymax, xmin, xmax;
	bool isSwitched;

public:
	GraphEdge(GraphNode* node1, GraphNode* node2, SDL_Color color, EdgeType type);
	GraphNode* getNode1();
	GraphNode* getNode2();
	SDL_Color getColor();
	void setColor(SDL_Color color);

	EdgeType getType();
	
	void update();
	bool containsNode(GraphNode* node);
	bool containsGate(Gate* gate);
	bool isFrom(GraphNode* node);
	bool isTo(GraphNode* node);
	bool isSwitchedOn();
	bool isSwitchTouched(int x, int y);
	bool isSwitchTouched(Vec2 pos);
	void toggleSwitch();
	void render(void);
	bool isTouched(int x, int y);
	bool isTouched(Vec2 pos);
};

