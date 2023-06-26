#pragma once
#include "SDL.h"
#include <string>
#include "Window.h"
#include "Input.h"
#include "Elevator.h"

using namespace std;

class Button
{

public:

	const int R = 255;
	const int G = 0;
	const int B = 0;

	const int R_HOVER = 0;
	const int G_HOVER = 255;
	const int B_HOVER = 0;

	string text;
	SDL_Rect rect = {0,0, 36,36};
	bool isHovering = false;

	int srcFloor;
	int dstFloor;

	void update()
	{
		isHovering = SDL_PointInRect(&Input::Mouse::point, &rect);
		bool isClicked = isHovering && Input::Mouse::buttonUp;
		if (isClicked)
		{
			Elevator::get().peopleWaiting.push_back(Person(srcFloor, dstFloor));
		}
	}

	void render()
	{
		if (isHovering)
		{
			render(R_HOVER, G_HOVER, B_HOVER);
		}
		else
		{
			render(R, G, B);
		}
	}

	void render(int r, int g, int b)
	{
		Window::drawRect(rect, r, g, b);
		SDL_Point centred = Window::Text::getCenteredTextPoint(rect, text);
		Window::Text::drawString(centred, text);
	}

};