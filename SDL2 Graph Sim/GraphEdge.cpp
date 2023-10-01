#include "GraphEdge.h"

GraphEdge::GraphEdge(GraphNode* node1, GraphNode* node2, SDL_Color color, EdgeType type) {
	this->node1 = node1;
	this->node2 = node2;
	this->color = color;
	this->type = type;
	
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

	SDL_RenderDrawLine(renderer, node1->getX(), node1->getY(), node2->getX(), node2->getY());

	double dx = to->getX() - from->getX();
	double dy = to->getY() - from->getY();

	double tx = to->getX(), ty = to->getY();

	double mag = sqrt(dx * dx + dy * dy);

	double rad = to->getRadius();

	dx /= mag; dy /= mag;
	tx -= rad * dx;
	ty -= rad * dy;

	for (int i = 10; i > 0; i--) {

		SDL_RenderDrawLine(renderer, (int)(tx - i * dx - i * dy),
			(int)(ty - i * dy + i * dx),
			(int)(tx - i * dx + i * dy),
			(int)(ty - i * dy - i * dx));
	}
}

void GraphEdge::renderNone()
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, node1->getX(), node1->getY(), node2->getX(), node2->getY());
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

bool GraphEdge::containsNode(GraphNode* node) {
	return node1 == node || node2 == node;
}

void GraphEdge::update() {
	//updates information about node1 and node2 after any movement.
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