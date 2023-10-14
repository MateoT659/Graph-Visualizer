#pragma once
#include "GraphSimConst.h"
class FreeEdge
{
private:
	Vec2 from;
	Vec2 to;
	SDL_Color color;
	EdgeType type;
	double unitX, unitY;

	void renderSwitch();
	void renderDotted();
	void renderDashed();
	void renderResist();
	void renderDirected();
	void renderNone();

	double slope;
	int b, ymin, ymax, xmin, xmax;
	bool isSwitched;

public:
	FreeEdge(Vec2 from, Vec2 to, SDL_Color color, EdgeType type);
	FreeEdge(int fx, int fy, int tx, int ty, SDL_Color color, EdgeType type);
	Vec2 getFrom();
	Vec2 getTo();
	SDL_Color getColor();
	void setColor(SDL_Color color);

	EdgeType getType();

	void translateBy(Vec2 vec);

	bool connectedTo(Vec2 point);
	bool isSwitchedOn();
	bool isSwitchTouched(int x, int y);
	bool isSwitchTouched(Vec2 pos);
	void toggleSwitch();
	void render(void);
	bool isTouched(int x, int y);
	bool isTouched(Vec2 pos);
	void update();
	FreeEdge* copy();
};

