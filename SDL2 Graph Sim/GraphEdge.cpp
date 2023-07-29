#include "GraphEdge.h"

GraphEdge::GraphEdge(GraphNode* node1, GraphNode* node2, SDL_Color color) {
	this->node1 = node1;
	this->node2 = node2;
	this->color = color;

	ymin = std::min(node1->getY(), node2->getY());
	ymax = std::max(node1->getY(), node2->getY());

	slope = (double)(node2->getY() - node1->getY()) / (node2->getX() - node1->getX());
	b = (int) - 1 * slope * node1->getX() + node1->getY();
}

void GraphEdge::render() {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, node1->getX(), node1->getY(), node2->getX(), node2->getY());
}

bool GraphEdge::isTouched(int x, int y) {
	return (y <= slope * x + b + 20 && y >= slope * x + b - 20) && (y>ymin && y<ymax);
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