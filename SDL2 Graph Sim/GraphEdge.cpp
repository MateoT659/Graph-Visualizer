#include "GraphEdge.h"

GraphEdge::GraphEdge(GraphNode* node1, GraphNode* node2, SDL_Color color) {
	this->node1 = node1;
	this->node2 = node2;
	this->color = color;
	
	//prevents division by zero
	if (node1->getX() == node2->getX()) {
		node1->setPos(node1->getX() + 1, node1->getY());
	}

	ymin = std::min(node1->getY(), node2->getY());
	ymax = std::max(node1->getY(), node2->getY());
	xmin = std::min(node1->getX(), node2->getX());
	xmax = std::max(node1->getX(), node2->getX());

	slope = (double)(node2->getY() - node1->getY()) / (node2->getX() - node1->getX());
	b = (int) (- 1 * slope * node1->getX() + node1->getY());
}

void GraphEdge::render() {
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

GraphNode* GraphEdge::getNode1() {
	return node1;
}

GraphNode* GraphEdge::getNode2() {
	return node2;
}

void GraphEdge::setColor(SDL_Color color) {
	this->color = color;
}

bool GraphEdge::hasNode(GraphNode* node) {
	return node1 == node || node2 == node;
}

void GraphEdge::update() {
	//updates information about node1 and node2 after any movement.
	if (node1->getX() == node2->getX()) {
		node1->setPos(node1->getX() + 1, node1->getY());
	}

	ymin = std::min(node1->getY(), node2->getY());
	ymax = std::max(node1->getY(), node2->getY());
	xmin = std::min(node1->getX(), node2->getX());
	xmax = std::max(node1->getX(), node2->getX());
	slope = (double)(node2->getY() - node1->getY()) / (node2->getX() - node1->getX());
	b = (int)(-1 * slope * node1->getX() + node1->getY());
}