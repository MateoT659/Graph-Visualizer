#pragma once
#include "GraphSimConst.h"
#include "GraphNode.h"
class Gate
{
protected:
	//interface for unary (not, none) and binary gates
	SDL_Color color;

public:
	virtual int getType() = 0;
	virtual int isMemberTouched(Vec2 pos) = 0; //to be called during edge testing
	virtual GraphNode* getNode(int index)=0;
	virtual bool isTouched(Vec2 pos)=0;
	virtual void setPos(Vec2 vec) = 0;
	virtual void translateBy(Vec2 pos)=0;
	void setColor(SDL_Color color) { this->color = color; };
	SDL_Color getColor() { return color; };
	virtual void render()=0;
	virtual void renderGhost() = 0;
	virtual Gate* copy() = 0;
	virtual bool containsNode(GraphNode* node) = 0;
	
};

class UnaryGate: public Gate {
private:
	Vec2 pos;
	GraphNode *in, *out;//in = 0, out = 1;
	uGateType type;

	void renderIden();
	void renderNot();

public:
	UnaryGate(Vec2 pos, SDL_Color color, uGateType type);
	~UnaryGate();
	int getType() { return (int)type; }
	int isMemberTouched(Vec2 pos); //to be called during edge testing
	GraphNode* getNode(int index);
	bool isTouched(Vec2 pos);
	void translateBy(Vec2 pos);
	void setPos(Vec2 pos);
	void render();
	void renderGhost();
	Gate* copy();
	bool containsNode(GraphNode* node);
};

class BinaryGate : public Gate {
private:
	Vec2 pos;
	GraphNode *in1, *in2, * out;//in = 0, out = 1;
	bGateType type;

	void renderAnd();
	void renderNand();
	void renderOr();
	void renderNor();
	void renderXor();
	void renderXnor();
public:
	BinaryGate(Vec2 pos, SDL_Color color, bGateType type);
	~BinaryGate();
	int getType() { return (int)type; }
	void setPos(Vec2 vec);
	int isMemberTouched(Vec2 pos); //to be called during edge testing
	GraphNode* getNode(int index);
	bool isTouched(Vec2 pos);
	void translateBy(Vec2 pos);
	void render();
	void renderGhost();
	Gate* copy();
	bool containsNode(GraphNode* node);
};