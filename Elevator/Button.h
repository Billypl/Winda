#pragma once
#include "SDL.h"
#include <string>
#include "Window.h"
#include "Input.h"

using namespace std;

class Button
{

public:

	const int r = 255;
	const int g = 0;
	const int b = 0;

	const int r_hover = 0;
	const int g_hover = 255;
	const int b_hover = 0;

	string text;
	SDL_Rect rect = {0,0, 36,36};
	bool isHovering = false;
	void (*callback) ();

	int srcFloor;
	int dstFloor;

	void render()
	{
		if (isHovering)
		{
			render(r_hover, g_hover, b_hover);
		}
		else
		{
			render(r, g, b);
		}
	}

	void setPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	void set(const string& text, int x, int y, int srcFloor, int dstFloor)
	{
		this->text = text;
		this->rect.x = x;
		this->rect.y = y;
		this->srcFloor = srcFloor;
		this->dstFloor = dstFloor;
	}

	void render(int r, int g, int b)
	{
		Window::drawRect(rect, r, g, b);
		SDL_Point centred = { rect.x + rect.w / 2 - text.length() * Window::Text::LETTER_SIZE / 2,
								rect.y + rect.h / 2 - Window::Text::LETTER_SIZE / 2 };
		Window::Text::drawString(centred.x, centred.y, text.c_str());
	}

	void update()
	{
		isHovering = SDL_PointInRect(&Input::Mouse::point, &rect);
		if (isHovering && Input::Mouse::buttonUp)
		{
			callback();
		}
	}

	
};