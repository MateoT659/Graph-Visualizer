#include "Gate.h"

UnaryGate::UnaryGate(Vec2 pos, SDL_Color color, uGateType type)
{
	this->pos = pos;
	this->in = new GraphNode(pos - Vec2(30, 0), 20, color, GATE);
	this->out = new GraphNode(pos + Vec2(38, 0), 20, color, GATE);
	this->color = color;
	this->type = type;
}

UnaryGate::~UnaryGate()
{
	delete in;
	delete out;
}

Vec2 UnaryGate::getPos()
{
	return pos;
}

int UnaryGate::getNodeInd(GraphNode* g)
{
	if (in == g) return 0;
	if (out == g) return 1;
	return -1;
}
bool UnaryGate::isTouched(Vec2 vec)
{
	return rectIsTouched({ pos.x - 15, pos.y - 20, 40, 40 }, vec);
}

void UnaryGate::translateBy(Vec2 vec){
	this->pos += vec;
	in->setPos(in->getPos() + vec);
	out->setPos(out->getPos() + vec);
}

void UnaryGate::setPos(Vec2 pos)
{
	this->pos = pos;
	in->setPos(pos - Vec2(30, 0));
	out->setPos(pos + Vec2(38, 0));

}

int UnaryGate::isMemberTouched(Vec2 pos) {
	//returns -1 if no member is touched, return index of node if touched

	if (in->containsPoint(pos)) {
		return 0;
	}
	if (out->containsPoint(pos)) {
		return 1;
	}

	return -1;
}

GraphNode* UnaryGate::getNode(int index)
{
	switch (index) {
	case 0:
		return in;
	case 1:
		return out;
	default:
		return nullptr;
	}
}


void UnaryGate::renderIden()
{
	//filled
	/*
	Vec2 slope(2, 0);//bottom

	Vec2 backanchor = pos - Vec2(15, 20);

	Vec2 inc = Vec2(0, 1);

	for (int i = 0; i < 20; i++) {
		drawLine(backanchor, backanchor + slope * i);
		backanchor += inc;
	}

	drawLine(backanchor, in->getPos());
	drawLine(backanchor, out->getPos());

	for (int i = 20; i >= 0; i--) {
		drawLine(backanchor, backanchor + slope * i);
		backanchor += inc;
	}
	*/
	//skeleton
	drawLine(pos - Vec2(15, 20), pos - Vec2(15, -20));
	drawLine(pos - Vec2(15, 20), pos + Vec2(25, 0));
	drawLine(pos - Vec2(15, -20), pos + Vec2(25, 0));
	drawLine(pos - Vec2(15, 0), in->getPos());
	drawLine(pos + Vec2(25, 0), out->getPos());
}

void UnaryGate::renderNot()
{
	drawLine(pos - Vec2(15, 20), pos - Vec2(15, -20));
	drawLine(pos - Vec2(15, 20), pos + Vec2(25, 0));
	drawLine(pos - Vec2(15, -20), pos + Vec2(25, 0));
	drawLine(pos - Vec2(15, 0), in->getPos());
	drawOpenCircle(pos + Vec2(28, 0), 4);
	drawLine(pos + Vec2(31, 0), out->getPos());

}


void UnaryGate::render()
{
	setRenderColor(color);
	switch (type) {
	case IDENTITY:
		renderIden();
		break;
	case NOT:
		renderNot();
		break;
	}
}

void UnaryGate::renderGhost()
{
	setRenderColor(color, 100);
	switch (type) {
	case IDENTITY:
		renderIden();
		break;
	case NOT:
		renderNot();
		break;
	}
}

Gate* UnaryGate::copy()
{
	return new UnaryGate(pos, color, type);
}

bool UnaryGate::containsNode(GraphNode* node)
{
	return in == node || out == node;
}

int UnaryGate::getNodeNum()
{
	return 2;
}

BinaryGate::BinaryGate(Vec2 pos, SDL_Color color, bGateType type)
{
	this->pos = pos;
	this->in1 = new GraphNode(pos - Vec2(26, 20), 20, color, GATE);
	this->in2 = new GraphNode(pos - Vec2(20, -20), 20, color, GATE);
	this->out = new GraphNode(pos + Vec2(60, 0), 20, color, GATE);
	this->color = color;
	this->type = type;
}
BinaryGate::~BinaryGate()
{
	delete in1;
	delete in2;
	delete out;
}

Vec2 BinaryGate::getPos()
{
	return pos;
}

int BinaryGate::getNodeInd(GraphNode* g)
{
	if (in1 == g) return 0;
	if (in2 == g) return 1;
	if (out == g) return 2;
	return -1;
}

void BinaryGate::setPos(Vec2 vec)
{
	this->pos = vec;
	in1->setPos(vec - Vec2(26, 20));
	in2->setPos(vec - Vec2(20, -20));
	out->setPos(vec + Vec2(60, 0));
}

int BinaryGate::isMemberTouched(Vec2 pos)
{
	if (in1->containsPoint(pos)) {
		return 0;
	}
	if (in2->containsPoint(pos)) {
		return 1;
	}
	if (out->containsPoint(pos)) {
		return 2;
	}
	return -1;
}

GraphNode* BinaryGate::getNode(int index)
{
	switch (index) {
	case 0:
		return in1;
	case 1:
		return in2;
	case 2:
		return out;
	default:
		return nullptr;
	}
}

bool BinaryGate::isTouched(Vec2 vec)
{
	return 30*30 > (pos + Vec2(15, 0) - vec).mag2();	
}

void BinaryGate::translateBy(Vec2 vec)
{
	pos += vec;
	in1->translateBy(vec);
	in2->translateBy(vec);
	out->translateBy(vec);
}

void BinaryGate::renderAnd()
{
	int RADIUS = 30;
	int x = pos.x+15, y = pos.y;
	int dx = RADIUS, dy = 0; 

	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dy, y + dx);
		drawPoint(x + dx, y - dy);
		drawPoint(x + dy, y - dx);
		dy++;
		while (RADIUS*RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}

	drawLine(pos + Vec2(0, 30), pos + Vec2(15, 30));
	drawLine(pos + Vec2(0, -30), pos + Vec2(15, -30));
	

	drawLine(pos + Vec2(0, RADIUS), pos - Vec2(0, RADIUS));
	drawLine(pos + Vec2(RADIUS+15, 0), out->getPos());
	drawLine(pos - Vec2(0, 20), in1->getPos());
	drawLine(pos + Vec2(0, 20), in2->getPos());
}

void BinaryGate::renderNand()
{
	int RADIUS = 30;
	int x = pos.x + 15, y = pos.y;
	int dx = RADIUS, dy = 0;

	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dy, y + dx);
		drawPoint(x + dx, y - dy);
		drawPoint(x + dy, y - dx);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}

	drawLine(pos + Vec2(0, 30), pos + Vec2(15, 30));
	drawLine(pos + Vec2(0, -30), pos + Vec2(15, -30));

	drawLine(pos + Vec2(0, RADIUS), pos - Vec2(0, RADIUS));
	
	drawOpenCircle(pos + Vec2(RADIUS + 18,0), 4);
	drawLine(pos + Vec2(RADIUS + 21, 0), out->getPos());
	drawLine(pos - Vec2(0, 20), in1->getPos());
	drawLine(pos + Vec2(0, 20), in2->getPos());
}

void BinaryGate::renderOr()
{
	int RADIUS = 30;
	int x = pos.x + 15, y = pos.y;
	int dx = RADIUS, dy = 0;

	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dy, y + dx);
		drawPoint(x + dx, y - dy);
		drawPoint(x + dy, y - dx);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
	drawLine(pos + Vec2(0, 30), pos + Vec2(15, 30));
	drawLine(pos + Vec2(0, -30), pos + Vec2(15, -30));
	
	//circle from far away??


	//drawOpenCircle(pos + Vec2(RADIUS + 18, 0), 4); //for N
	drawLine(pos + Vec2(RADIUS + 15, 0), out->getPos());
	drawLine(pos - Vec2(-4, 20), in1->getPos());
	drawLine(pos + Vec2(4, 20), in2->getPos());

	RADIUS = 50;
	x = pos.x - 40, y = pos.y;
	dx = RADIUS, dy = 0;

	while (dy<=30) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dx, y - dy);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}

}

void BinaryGate::renderNor()
{
	int RADIUS = 30;
	int x = pos.x + 15, y = pos.y;
	int dx = RADIUS, dy = 0;

	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dy, y + dx);
		drawPoint(x + dx, y - dy);
		drawPoint(x + dy, y - dx);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
	drawLine(pos + Vec2(0, 30), pos + Vec2(15, 30));
	drawLine(pos + Vec2(0, -30), pos + Vec2(15, -30));

	//circle from far away??


	drawOpenCircle(pos + Vec2(RADIUS + 18, 0), 4); //for N
	drawLine(pos + Vec2(RADIUS + 21, 0), out->getPos());
	drawLine(pos - Vec2(-4, 20), in1->getPos());
	drawLine(pos + Vec2(4, 20), in2->getPos());

	RADIUS = 50;
	x = pos.x - 40, y = pos.y;
	dx = RADIUS, dy = 0;
	while (dy <= 30) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dx, y - dy);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
}

void BinaryGate::renderXor()
{
	int RADIUS = 30;
	int x = pos.x + 15, y = pos.y;
	int dx = RADIUS, dy = 0;

	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dy, y + dx);
		drawPoint(x + dx, y - dy);
		drawPoint(x + dy, y - dx);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
	drawLine(pos + Vec2(0, 30), pos + Vec2(15, 30));
	drawLine(pos + Vec2(0, -30), pos + Vec2(15, -30));

	//circle from far away??


	//drawOpenCircle(pos + Vec2(RADIUS + 18, 0), 4); //for N
	drawLine(pos + Vec2(RADIUS + 15, 0), out->getPos());
	drawLine(pos - Vec2(-4, 20), in1->getPos());
	drawLine(pos + Vec2(4, 20), in2->getPos());

	RADIUS = 50;
	x = pos.x - 40, y = pos.y;
	dx = RADIUS, dy = 0;

	while (dy <= 30) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dx, y - dy);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
	x = pos.x - 48, y = pos.y;
	dx = RADIUS, dy = 0;

	while (dy <= 30) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dx, y - dy);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
}

void BinaryGate::renderXnor()
{
	int RADIUS = 30;
	int x = pos.x + 15, y = pos.y;
	int dx = RADIUS, dy = 0;

	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dy, y + dx);
		drawPoint(x + dx, y - dy);
		drawPoint(x + dy, y - dx);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
	drawLine(pos + Vec2(0, 30), pos + Vec2(15, 30));
	drawLine(pos + Vec2(0, -30), pos + Vec2(15, -30));

	drawOpenCircle(pos + Vec2(RADIUS + 18, 0), 4); //for N
	drawLine(pos + Vec2(RADIUS + 21, 0), out->getPos());
	drawLine(pos - Vec2(-4, 20), in1->getPos());
	drawLine(pos + Vec2(4, 20), in2->getPos());

	RADIUS = 50;
	x = pos.x - 40, y = pos.y;
	dx = RADIUS, dy = 0;

	while (dy <= 30) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dx, y - dy);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
	x = pos.x - 48, y = pos.y;
	dx = RADIUS, dy = 0;

	while (dy <= 30) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dx, y - dy);
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
}

void BinaryGate::render()
{
	setRenderColor(color);
	switch (type) {
	case AND:
		renderAnd();
		break;
	case NAND:
		renderNand();
		break;
	case OR:
		renderOr();
		break;
	case NOR:
		renderNor();
		break;
	case XOR:
		renderXor();
		break;
	case XNOR:
		renderXnor();
		break;
	}
}
void BinaryGate::renderGhost()
{
	setRenderColor(color, 100);
	switch (type) {
	case AND:
		renderAnd();
		break;
	case NAND:
		renderNand();
		break;
	case OR:
		renderOr();
		break;
	case NOR:
		renderNor();
		break;
	case XOR:
		renderXor();
		break;
	case XNOR:
		renderXnor();
		break;
	}
}

Gate* BinaryGate::copy()
{
	return new BinaryGate(pos, color, type);
}

bool BinaryGate::containsNode(GraphNode* node)
{
	return in1 == node || in2 == node || out == node;
}

int BinaryGate::getNodeNum()
{
	return 3;
}
