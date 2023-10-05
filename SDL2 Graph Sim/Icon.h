#pragma once
#include "Image.h"
class Icon
{
private:
	Image *icon;
	Image *selected;
	Image *hovered;
	bool *select;
	bool hover;

public:
	Icon(int x, int y, std::string icon);

	void setIcon(std::string filepath);

	bool containsPoint(Vec2 point);

	bool* getSelectedAddress();

	bool isHovered();
	bool isSelected();

	void setHover(bool value);
	void setSelected(bool value);

	void toggleSelected();

	void render();

	void linkTo(Icon *other);

};

