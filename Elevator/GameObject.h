#pragma once

#include <SDL.h>
#include <string>
#include "Window.h"
using namespace std;

class GameObject
{

public:

	SDL_Texture* texture;
	SDL_Rect rect;
	GameObject() {};

	void set(SDL_Texture* texture, SDL_Rect rect)
	{
		this->texture = texture;
		this->rect = rect;
	}
	void set(const string& filename, SDL_Rect rect)
	{
		this->texture = Window::loadIMG(filename);
		this->rect = rect;
	}

	void update(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
	void render()
	{
		SDL_RenderCopy(Window::renderer, texture, NULL, &rect);
	}

};