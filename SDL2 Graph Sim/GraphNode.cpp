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
	switch (type) {
	case Filled:
	case Skeleton:
	case Cross:
		return (pos - point).mag2() <= radius * radius;
	case FilledSq:
	case OpenSq:
	case CrossSq:
		return rectIsTouched({ pos.x - radius, pos.y - radius, 2 * radius,2 * radius }, point);
	}
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

	//line solution (MUAH beautiful)

	int x = pos.x, y = pos.y;
	int dx = radius, dy = 0, prevdx = -1;

	while (dx >= dy) {
		drawLine(x + dx, y + dy, x - dx, y + dy);
		if (dy != 0)
			drawLine(x + dx, y - dy, x - dx, y - dy);

		if (prevdx != dx && dy>0 && dy != dx) {
			drawLine(x + dy, y + dx, x - dy, y + dx);
			drawLine(x + dy, y - dx, x - dy, y - dx);
			prevdx = dx;
		}
		dy++;
		while (radius * radius < dx * dx + dy * dy) {
			dx--;
		}

	}

	//flood fill solution (surprisingly bad, but i learned how to hash so not all that bad)
	/*
	//make skeleton and flood fill using queue solution

	std::unordered_set<Vec2> filled;
	std::queue<Vec2> toFill;

	//render outline (skeleton) and add all points to filled points.

	int x = pos.x, y = pos.y;
	int dx = radius, dy = 0;
	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		filled.insert(Vec2(x + dx, y + dy));
		drawPoint(x + dy, y + dx);
		filled.insert(Vec2(x + dy, y + dx));
		drawPoint(x - dx, y + dy);
		filled.insert(Vec2(x - dx, y + dy));
		drawPoint(x - dy, y + dx);
		filled.insert(Vec2(x - dy, y + dx));
		drawPoint(x + dx, y - dy);
		filled.insert(Vec2(x + dx, y - dy));
		drawPoint(x + dy, y - dx);
		filled.insert(Vec2(x + dy, y - dx));
		drawPoint(x - dx, y - dy);
		filled.insert(Vec2(x - dx, y - dy));
		drawPoint(x - dy, y - dx);
		filled.insert(Vec2(x - dy, y - dx));

		dy++;
		while (radius * radius < dx * dx + dy * dy) {
			dx--;
		}
	}


	Vec2 zeroone(0, 1);
	Vec2 onezero(1, 0);
	Vec2 curr;
	//then, begin flood-filling using a queue from the central point. This avoids doing any work on pixels outside the circle.
	toFill.push(pos);
	while (!toFill.empty()) {
		curr = toFill.front();
		toFill.pop();
		drawPoint(curr);
		if (filled.count(curr + zeroone) == 0) {
			toFill.push(curr + zeroone);
			filled.insert(curr + zeroone);
		}
		if (filled.count(curr - zeroone) == 0) {
			toFill.push(curr - zeroone);
			filled.insert(curr - zeroone);
		}
		if (filled.count(curr + onezero) == 0) {
			toFill.push(curr + onezero);
			filled.insert(curr + onezero);
		}
		if (filled.count(curr - onezero) == 0) {
			toFill.push(curr - onezero);
		}

	}
	*/

	//original double for loop if statement solution (ok)
	/* for (int w = 0; w < radius * 2; w++)
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
	}*/
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

void GraphNode::renderCrossed()
{
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

	double rnorm = radius / sqrt2;

	drawLine((int)(pos.x - rnorm+1),(int)( pos.y-rnorm+1),(int) (pos.x+ rnorm), (int)(pos.y + rnorm));
	drawLine((int)(pos.x  +rnorm -1), (int)(pos.y - rnorm +1), (int)(pos.x - rnorm +1),(int)(pos.y + rnorm-1));
}

void GraphNode::renderFilledSq()
{
	drawFilledRectangle(pos.x - radius, pos.y - radius, 2 * radius, 2 * radius);
}

void GraphNode::renderSkeletonSq()
{
	SDL_Rect rect = { pos.x - radius, pos.y - radius, 2 * radius, 2 * radius };
	SDL_RenderDrawRect(renderer, &rect);
}

void GraphNode::renderCrossedSq()
{
	SDL_Rect rect = { pos.x - radius, pos.y - radius, 2 * radius, 2 * radius };
	SDL_RenderDrawRect(renderer, &rect);
	drawLine(pos.x - radius, pos.y - radius, pos.x + radius, pos.y + radius);
	drawLine(pos.x + radius, pos.y - radius, pos.x - radius, pos.y + radius);
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
	case Cross:
		renderCrossed();
		break;
	case FilledSq:
		renderFilledSq();
		break;
	case OpenSq:
		renderSkeletonSq();
		break;
	case CrossSq:
		renderCrossedSq();
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
