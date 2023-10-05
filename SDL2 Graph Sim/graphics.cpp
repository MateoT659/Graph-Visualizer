#include "Includes.h"


void setRenderColor(SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
void setRenderColor(SDL_Color color, int a) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, a);
}
void setRenderColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void clearScreen(SDL_Color color) {
	setRenderColor(color);
	SDL_RenderClear(renderer);
}

void drawPoint(int x, int y) {
	SDL_RenderDrawPoint(renderer, x, y);
}
void drawPoint(Vec2 pos) {
	SDL_RenderDrawPoint(renderer, pos.x, pos.y);
}

void drawLine(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
void drawLine(Vec2 point1, Vec2 point2) {
	SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
}

void drawFilledRectangle(SDL_Rect rect, SDL_Color color) {
	setRenderColor(color);

	while (rect.w > 0 || rect.h > 0) {
		SDL_RenderDrawRect(renderer, &rect);
		rect.x++;
		rect.y++;
		rect.h -= 2;
		rect.w -= 2;
	}
}
void drawArrow(int x1, int y1, int x2, int y2, SDL_Color color) {
	setRenderColor(color);

	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

	double dx = x2 - x1;
	double dy = y2 - y1;

	double tx = x2, ty = y2;

	double mag = sqrt(dx * dx + dy * dy);

	dx /= mag; dy /= mag;

	for (int i = 10; i > 0; i--) {

		SDL_RenderDrawLine(renderer, (int)(tx - i * dx - i * dy),
			(int)(ty - i * dy + i * dx),
			(int)(tx - i * dx + i * dy),
			(int)(ty - i * dy - i * dx));
	}
}

void updateIcons() {
	switch ((int)edgeType) {
	case 0:
		icons[1]->setIcon("Assets/IconEdge.png");
		break;
	case 1:
		icons[1]->setIcon("Assets/IconArrow.png");
		break;
	}

	switch ((int)ghost->getType()) {
	case 0:
		icons[0]->setIcon("Assets/IconFilledCircle.png");
		break;
	case 1:
		icons[0]->setIcon("Assets/IconOpenCircle.png");
		break;
	}
}

SDL_Texture* loadTexture(std::string filepath) {
	//returns nullptr on error;
	SDL_Texture* texture = NULL;

	
	SDL_Surface* loadedSurface = IMG_Load(filepath.c_str());

	if (loadedSurface == NULL)
	{
		std::cout << "Error: Couldn't load image!\n";
		return nullptr;
	}

	//Create texture from surface pixels
	texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (texture == NULL)
	{
		std::cout << "Error: Couldn't make texture from image!\n";
		return nullptr;
	}

	SDL_FreeSurface(loadedSurface);
	return texture;
}

void render(bool showGhost) {
	clearScreen(BLACK);

	for (int i = 0; i < edges.size(); i++) {
		edges[i]->render();
	}

	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->render();
	}

	if (showGhost)
		ghost->render();

	sidebar->render();

	for (int i = 0; i < icons.size(); i++) {
		icons[i]->render();
	}


	drawFilledRectangle(colorBox, currentColor);
}

void renderU(bool showGhost) {
	render(showGhost);
	SDL_RenderPresent(renderer);
}