#include "FreeEdge.h"

FreeEdge::FreeEdge(Vec2 from, Vec2 to, SDL_Color color, EdgeType type)
{
	this->from = from;
	this->to = to;
	this->color = color;
	this->type = type;
	isSwitched = false;

	Vec2 line = to - from;
	double mag = line.mag();
	this->unitX = line.x / mag;
	this->unitY = line.y / mag;

	ymin = min(to.y, from.y);
	ymax = max(to.y, from.y);
	xmin = min(to.x, from.x);
	xmax = max(to.x, from.x);
	slope = (double)(to.y - from.y) / (to.x - from.x + (to.x == from.x ? 1 : 0));

	b = (int)(from.y - slope * from.x);
	
}

FreeEdge::FreeEdge(int fx, int fy, int tx, int ty, SDL_Color color, EdgeType type)
{
	from.x = fx;
	from.y = fy;
	to.x = tx;
	to.y = ty;
	this->color = color;
	this->type = type;
	isSwitched = false;

	Vec2 line = to - from;
	double mag = line.mag();
	this->unitX = line.x / mag;
	this->unitY = line.y / mag;

	ymin = min(to.y, from.y);
	ymax = max(to.y, from.y);
	xmin = min(to.x, from.x);
	xmax = max(to.x, from.x);

	slope = (double)(to.y - from.y) / (to.x - from.x + (to.x == from.x ? 1 : 0));
	b = (int)(from.y - slope * from.x);
}

Vec2 FreeEdge::getFrom()
{
	return from;
}

Vec2 FreeEdge::getTo()
{
	return to;
}

SDL_Color FreeEdge::getColor()
{
	return color;
}

void FreeEdge::setColor(SDL_Color color)
{
	this->color = color;
}

EdgeType FreeEdge::getType()
{
	return type;
}

void FreeEdge::translateBy(Vec2 vec)
{
	this->from += vec;
	this->to += vec;
	update();
}

bool FreeEdge::connectedTo(Vec2 point)
{
	return point == from || point == to;
}

bool FreeEdge::isSwitchedOn()
{
	return isSwitched;
}

bool FreeEdge::isSwitchTouched(int x, int y)
{
	Vec2 mid = (from + to) / 2;
	return (mid.x - x) * (mid.y - y) <= 30 * 30;
}

bool FreeEdge::isSwitchTouched(Vec2 pos)
{
	Vec2 mid = (from + to) / 2;
	return (mid - pos).mag2() <= 30 * 30;
}

void FreeEdge::toggleSwitch()
{
	isSwitched = !isSwitched;
}

void FreeEdge::render(void)
{
	setRenderColor(color);
	switch (type) {
	case None:
		renderNone();
		break;
	case Directed:
		renderDirected();
		break;
	case Resistance:
		renderResist();
		break;
	case Dashed:
		renderDashed();
		break;
	case Dotted:
		renderDotted();
		break;
	case Switch:
		renderSwitch();
	}
}


void FreeEdge::renderSwitch()
{
	Vec2 v((int)(unitX * 60), (int)(unitY * 60));
	Vec2 line = (to - from - v) / 2;
	drawCircle(from + line, 3);
	drawCircle(to - line, 3);

	if (isSwitched) {
		drawLine(from, to);
	}
	else {
		drawLine(from, from + line);
		drawLine(to, to - line);
		drawLine(from + line, from + line + v / (1.35) - v.getPerp() / (2));
	}
}

void FreeEdge::renderDotted()
{
	double dx = from.x, dy = from.y;
	double ix = unitX * 6, iy = unitY * 6;

	while ((dy >= ymin && dy <= ymax) && (dx >= xmin && dx <= xmax)) {
		drawPoint((int)dx, (int)dy);
		dx += ix;
		dy += iy;
	}
}

void FreeEdge::renderDashed()
{
	double bx = from.x, by = from.y;
	double ex = from.x, ey = from.y;
	ex += (unitX * 8);
	ey += (unitY * 8);
	double incx = 2 * (ex - bx);
	double incy = 2 * (ey - by);

	while ((ey >= ymin && ey <= ymax) && (ex >= xmin && ex <= xmax)){
		drawLine((int)bx, (int)by, (int)ex, (int)ey);
		ex += incx;
		bx += incx;
		ey += incy;
		by += incy;
	}
}

void FreeEdge::renderResist()
{
	Vec2 v((int)(unitX * 5), (int)(unitY * 5));
	//adjust to change size;

	Vec2 line = (to - from) / 2 - v * 4;
	drawLine(from, from + line);
	drawLine(to, to - line);
	Vec2 sqStart = from + line;

	Vec2 perp(-(int)(unitY * 10), (int)(unitX * 10));
	drawLine(sqStart, sqStart + perp + v);
	drawLine(sqStart + perp + v, sqStart - perp + v * 3);
	drawLine(sqStart - perp + v * 3, sqStart + perp + v * 5);
	drawLine(sqStart + perp + v * 5, sqStart - perp + v * 7);
	drawLine(sqStart - perp + v * 7, to - line);
}

void FreeEdge::renderDirected()
{

	//idea how about i just store the unit vector in the edge so i dont have to recalculate it every single time i render

	double tx = to.x, ty = to.y;

	for (int i = 10; i > 0; i--) {

		SDL_RenderDrawLine(renderer, (int)(tx - i * unitX - i * unitY),
			(int)(ty - i * unitY + i * unitX),
			(int)(tx - i * unitX + i * unitY),
			(int)(ty - i * unitY - i * unitX));
	}
	drawLine(to, from);

}

void FreeEdge::renderNone()
{
	drawLine(from, to);
}

bool FreeEdge::isTouched(int x, int y)
{
	if (slope > 0) {
		return  ((y >= ymin-10 && y <= ymax+10) && (x >= xmin-10 && x <= xmax+10)) && ((y <= slope * (x + 12) + b + 12 && y >= slope * (x - 12) + b - 12));
	}
	else {
		return  ((y >= ymin-10 && y <= ymax+10) && (x >= xmin-10 && x <= xmax+10)) && ((y <= slope * (x - 12) + b + 12 && y >= slope * (x + 12) + b - 12));
	}
}

bool FreeEdge::isTouched(Vec2 v) {
	return isTouched(v.x, v.y);
}

void FreeEdge::update()
{
	ymin = min(to.y, from.y);
	ymax = max(to.y, from.y);
	xmin = min(to.x, from.x);
	xmax = max(to.x, from.x);
	b = (int)(from.y - slope * from.x);
}

FreeEdge* FreeEdge::copy()
{
	FreeEdge* ret =  new FreeEdge(from, to, color, type);
	if (isSwitched) {
		ret->toggleSwitch();
	}
	return ret;
}
