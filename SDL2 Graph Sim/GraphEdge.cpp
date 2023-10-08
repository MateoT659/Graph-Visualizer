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

	double fromX = node1->getX();
	double fromY = node1->getY();

	switch (node1->getType()) {
	case FilledSq:
	case OpenSq:
	case CrossSq:
		while (node1->containsPoint((int)fromX, (int)fromY)) {
			fromX += unitX;
			fromY += unitY;
		}
		break;
	default:
		fromX += unitX * node1->getRadius();
		fromY += unitY * node1->getRadius();
		break;
	}

	this->from.x = (int)fromX;
	this->from.y = (int)fromY;

	double toX = node2->getX();
	double toY = node2->getY();

	switch (node2->getType()) {
	case FilledSq:
	case OpenSq:
	case CrossSq:
		while (node2->containsPoint((int)toX, (int)toY)) {
			toX -= unitX;
			toY -= unitY;
		}
		break;
	default:
		toX -= unitX * node2->getRadius();
		toY -= unitY * node2->getRadius();
		break;
	}

	this->to.x = (int)toX;
	this->to.y = (int)toY;


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

	//calculate to and from vectors in update and constructor, then use them in specific renders
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

	//idea how about i just store the unit vector in the edge so i dont have to recalculate it every single time i render

	double tx = node2->getX(), ty = node2->getY();

	double rad = node2->getRadius();

	tx -= rad * unitX;
	ty -= rad * unitY;

	for (int i = 10; i > 0; i--) {

		SDL_RenderDrawLine(renderer, (int)(tx - i * unitX - i * unitY),
			(int)(ty - i * unitY + i * unitX),
			(int)(tx - i * unitX + i * unitY),
			(int)(ty - i * unitY - i * unitX));
	}
	drawLine(to, from);

}

void GraphEdge::renderNone()
{
	drawLine(from, to);
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

	double fromX = node1->getX(), fromY = node1->getY();
	switch (node1->getType()) {
	case FilledSq:
	case OpenSq:
	case CrossSq:
		while (node1->containsPoint((int)fromX, (int)fromY)) {
			fromX += unitX;
			fromY += unitY;
		}
		break;
	default:
		fromX += unitX * node1->getRadius();
		fromY += unitY * node1->getRadius();
		break;
	}

	this->from.x = (int)fromX;
	this->from.y = (int)fromY;

	double toX = node2->getX();
	double toY = node2->getY();

	switch (node2->getType()) {
	case FilledSq:
	case OpenSq:
	case CrossSq:
		while (node2->containsPoint((int)toX, (int)toY)) {
			toX -= unitX;
			toY -= unitY;
		}
		break;
	default:
		toX -= unitX * node2->getRadius();
		toY -= unitY * node2->getRadius();
		break;
	}

	this->to.x = (int)toX;
	this->to.y = (int)toY;

}