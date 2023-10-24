#pragma once
#include "GraphSimConst.h"
#include "GraphNode.h"
class NodeText
{
private:
	GraphNode* node;
	std::string text;
	SDL_Texture* texture;
	SDL_Rect box;
	SDL_Color color;
	bool editing;
public:
	NodeText(GraphNode* node, std::string text);

	GraphNode* getNode();

	bool isTouched(Vec2 pos);
	void setText(std::string text);
	std::string getText();

	void setState(bool state);
	bool getState();

	void updatePos();
	void updateColor();
	void render();
};