#include "GraphNode.h"

GraphNode::GraphNode(int x, int y, int radius, SDL_Color color) {
	this->x = x;
	this->y = y;
	this->radius = radius;
	this->color = color;
}

void GraphNode::setPos(int x, int y) {
	this->x = x;
	this->y = y;
}

void GraphNode::setColor(SDL_Color color) {
	this->color = color;
}

bool GraphNode::cotainsPoint(int xPos, int yPos) {
	return (x - xPos)* (x - xPos) + (y - yPos) * (y - yPos) <= radius * radius;
}

void GraphNode::render()
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
}

void GraphNode::renderSkeleton() {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	int dx = radius, dy = 0;

	while (dx >= dy) {
		SDL_RenderDrawPoint(renderer, x + dx, y + dy);
		SDL_RenderDrawPoint(renderer, x + dy, y + dx);
		SDL_RenderDrawPoint(renderer, x - dx, y + dy);
		SDL_RenderDrawPoint(renderer, x - dy, y + dx);
		SDL_RenderDrawPoint(renderer, x + dx, y - dy);
		SDL_RenderDrawPoint(renderer, x + dy, y - dx);
		SDL_RenderDrawPoint(renderer, x - dx, y - dy);
		SDL_RenderDrawPoint(renderer, x - dy, y - dx);
		dy++;
		while (radius * radius < dx * dx + dy * dy) {
			dx--;
		}
	}
}

int GraphNode::getX() {
	return x;
}

int* GraphNode::getXaddr(){
	return &x;
}

int GraphNode::getY() {
	return y;
}

int* GraphNode::getYaddr() {
	return &y;
}

int GraphNode::getRadius()
{
	return radius;
}
