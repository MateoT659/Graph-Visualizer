#include "Textbox.h"

Textbox::Textbox(std::string text, Vec2 pos, int height, SDL_Color color)
{
	this->text = text;
	this->box.x = pos.x;
	this->box.y = pos.y;
	this->box.h = height;
	this->box.w = (int)((double)height * 3. / 5. * (double)text.size());
	this->color = color;

	this->texture = NULL;
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface == NULL)
	{
		std::cout << "Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << "\n";
		SDL_FreeSurface(textSurface);
		return;
	}

	this->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (texture == NULL)
	{
		std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	}

	SDL_FreeSurface(textSurface);
}

Textbox::Textbox(std::string text, int x, int y, int height, SDL_Color color)
{
	if (text == "") text = " ";
	this->text = text;
	this->box.x = x;
	this->box.y = y;
	this->box.h = height;
	this->box.w = (int)((double)height * 3. / 5. * (double)text.size());
	this->color = color;

	this->texture = NULL;
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface == NULL)
	{
		std::cout << "Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << "\n";
		SDL_FreeSurface(textSurface);
		return;
	}

	this->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (texture == NULL)
	{
		std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	}

	SDL_FreeSurface(textSurface);
}


Textbox::~Textbox()
{
	SDL_DestroyTexture(texture);
	text.clear();
}

void Textbox::setText(std::string text)
{
	if (text == "") text = " ";
	if (this->text != text) {
		this->text = text;
		this->box.w = (int)((double)this->box.h * 3. / 5. * (double)text.size());
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), this->color);
		if (textSurface == NULL)
		{
			std::cout << "Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << "\n";
			SDL_FreeSurface(textSurface);
			return;
		}
		SDL_DestroyTexture(this->texture);
		this->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
		}

		SDL_FreeSurface(textSurface);
	}
}

std::string Textbox::getText()
{
	return text;
}

void Textbox::setColor(SDL_Color color)
{
	if (!colorEquals(this->color, color)) {
		this->color = color;
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
		if (textSurface == NULL)
		{
			std::cout << "Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << "\n";
			SDL_FreeSurface(textSurface);
			return;
		}

		this->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
		}

		SDL_FreeSurface(textSurface);
	}
}

SDL_Color Textbox::getColor() {
	return color;
}

void Textbox::setEditState(bool state)
{
	beingEdited = state;
}

bool Textbox::getEditState() {
	return beingEdited;
}

void Textbox::setPos(Vec2 pos)
{
	box.x = pos.x;
	box.y = pos.y;
}

void Textbox::setPos(int x, int y)
{
	box.x = x;
	box.y = y;
}

Vec2 Textbox::getPos() {
	return Vec2(box.x, box.y);
}

void Textbox::translateBy(Vec2 pos)
{
	box.x += pos.x;
	box.y += pos.y;
}

void Textbox::setHeight(int height)
{
	if (height < 15) height = 15;
	if (height > 300) height = 300;
	box.h = height;
	box.w = (int)((double)height * 3. / 5. * (double)text.size());
}

int Textbox::getHeight()
{
	return box.h;
}

bool Textbox::containsPoint(Vec2 point)
{
	return rectIsTouched(box, point);
}

void Textbox::render()
{
	if (beingEdited) {
		drawOpenRectangle(box, color);
	}
	SDL_RenderSetViewport(renderer, &box);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderSetViewport(renderer, nullptr);
}
