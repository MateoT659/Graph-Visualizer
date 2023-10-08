#include "GraphEdge.h"

GraphEdge::GraphEdge(GraphNode* node1, GraphNode* node2, SDL_Color color, EdgeType type) {
	this->node1 = node1;
	this->node2 = node2;
	this->color = color;
	this->type = type;
	
	Vec2 line = node2->getPos() - node1->getPos();

	double mag = line.mag();

	this->unitX = line.x / mag;
	this->unitY = line.y / mag;


	//prevents division by zero
	if (node1->getX() == node2->getX()) {
		node1->setPos(node1->getX() + 1, node1->getY());
	}

	ymin = min(node1->getY(), node2->getY());
	ymax = max(node1->getY(), node2->getY());
	xmin = min(node1->getX(), node2->getX());
	xmax = max(node1->getX(), node2->getX());

	slope = (double)(node2->getY() - node1->getY()) / (node2->getX() - node1->getX());
	b = (int) (- 1 * slope * node1->getX() + node1->getY());
}

void GraphEdge::render() {
	setRenderColor(color);
	switch (type) {
	case None:
		renderNone();
		break;
	case Directed:
		renderDirected();
		break;
	}
}

void GraphEdge::renderDirected()
{
	GraphNode* from = node1;
	GraphNode* to = node2;

	//idea how about i just store the unit vector in the edge so i dont have to recalculate it every single time i render

	double tx = to->getX(), ty = to->getY();

	double rad = to->getRadius();

	tx -= rad * unitX;
	ty -= rad * unitY;

	for (int i = 10; i > 0; i--) {

		SDL_RenderDrawLine(renderer, (int)(tx - i * unitX - i * unitY),
			(int)(ty - i * unitY + i * unitX),
			(int)(tx - i * unitX + i * unitY),
			(int)(ty - i * unitY - i * unitX));
	}
	drawLine((int)(to->getX() - unitX * rad), (int)(to->getY() - unitY * rad), (int)(from->getX() + unitX * from->getRadius()), (int)(from->getY() + unitY * from->getRadius()));

}

void GraphEdge::renderNone()
{
	drawLine((int)(node2->getX() - unitX * node2->getRadius()),
		(int)(node2->getY() - unitY * node2->getRadius()),
		(int)(node1->getX() + unitX * node1->getRadius()),
		(int)(node1->getY() + unitY * node1->getRadius()));
}

bool GraphEdge::isTouched(int x, int y) {
	if (slope > 0) {
		return ((y <= slope * (x + 12) + b + 12 && y >= slope * (x - 12) + b - 12)) && ((y > ymin && y < ymax) || (x > xmin && x < xmax));
	}
	else {
		return ((y <= slope * (x - 12) + b + 12 && y >= slope * (x + 12) + b - 12)) && ((y > ymin && y < ymax) || (x > xmin && x < xmax));
	}
}

bool GraphEdge::isTouched(Vec2 pos) {
	return isTouched(pos.x, pos.y);
}

GraphNode* GraphEdge::getNode1() {
	return node1;
}

GraphNode* GraphEdge::getNode2() {
	return node2;
}
SDL_Color GraphEdge::getColor() {
	return color;
}
void GraphEdge::setColor(SDL_Color color) {
	this->color = color;
}

EdgeType GraphEdge::getType()
{
	return type;
}

bool GraphEdge::containsNode(GraphNode* node) {
	return node1 == node || node2 == node;
}

void GraphEdge::update() {
	//updates information about node1 and node2 after any movement.

	Vec2 line = node2->getPos() - node1->getPos();

	double mag = line.mag();

	this->unitX = line.x / mag;
	this->unitY = line.y / mag;


	if (node1->getX() == node2->getX()) {
		node1->setPos(node1->getX() + 1, node1->getY());
	}

	ymin = min(node1->getY(), node2->getY());
	ymax = max(node1->getY(), node2->getY());
	xmin = min(node1->getX(), node2->getX());
	xmax = max(node1->getX(), node2->getX());
	slope = (double)(node2->getY() - node1->getY()) / (node2->getX() - node1->getX());
	b = (int)(-1 * slope * node1->getX() + node1->getY());
}