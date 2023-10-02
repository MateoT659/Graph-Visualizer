#include "GraphNode.h"

GraphNode::GraphNode(int x, int y, int radius, SDL_Color color, NodeType type) {
	this->pos.set(x, y);
	
	if (radius > 35) {
		this->radius = 35;
	}
	else if (radius < 5) {
		this->radius = 5;
	}
	else {
		this->radius = radius;
	}

	this->color = color;
	this->type = type;
	ghost = false;
}

GraphNode::GraphNode(Vec2 pos, int radius, SDL_Color color, NodeType type)
{
	this->pos = pos;
	this->radius = radius;
	this->color = color;
	this->type = type;
	ghost = false;
}
GraphNode* GraphNode::copy()
{
	return new GraphNode(pos, radius, color, type);
}

void GraphNode::setPos(int x, int y) {
	pos.set(x, y);
}

void GraphNode::setPos(Vec2 pos)
{
	this->pos = pos;
}

SDL_Color GraphNode::getColor()
{
	return color;
}

void GraphNode::setColor(SDL_Color color) {
	this->color = color;
}

NodeType GraphNode::getType()
{
	return type;
}

void GraphNode::setType(NodeType type)
{
	this->type = type;
}

void GraphNode::toggleGhost()
{
	ghost = !ghost;
}

bool GraphNode::containsPoint(Vec2 point)
{
	return (pos - point).mag2() <= radius * radius;
}

bool GraphNode::containsPoint(int xPos, int yPos) {

	return containsPoint(Vec2(xPos, yPos));
}

Vec2 GraphNode::getPos()
{
	return pos;
}



void GraphNode::renderFilled()
{
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				drawPoint((int)pos.x + dx, (int)pos.y + dy);
			}
		}
	}
}

void GraphNode::renderSkeleton() {
	int x = pos.x, y = pos.y;
	int dx = radius, dy = 0;

	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dy, y + dx);
		drawPoint(x - dx, y + dy);
		drawPoint(x - dy, y + dx);
		drawPoint(x + dx, y - dy);
		drawPoint(x + dy, y - dx);
		drawPoint(x - dx, y - dy);
		drawPoint(x - dy, y - dx);
		dy++;
		while (radius * radius < dx * dx + dy * dy) {
			dx--;
		}
	}
}

void GraphNode::render() {
	setRenderColor(color, ghost ? 100:255);
	
	switch (type) {
	case Filled:
		renderFilled();
		break;
	case Skeleton:
		renderSkeleton();
		break;
	}


}

int GraphNode::getX() {
	return pos.x;
}

int* GraphNode::getXaddr(){
	return (int*) & (pos.x);
}

int GraphNode::getY() {
	return pos.y;
}

int* GraphNode::getYaddr() {
	return (int*) &(pos.y);
}

int GraphNode::getRadius()
{
	return radius;
}

void GraphNode::setRadius(int radius)
{
	if (radius > 35) {
		this->radius = 35;
	}
	else if (radius < 5) {
		this->radius = 5;
	}
	else {
		this->radius = radius;
	}
}
