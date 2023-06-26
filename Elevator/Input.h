#pragma once
#include "SDL.h"
#include <iostream>

class Input
{

public:


	class Mouse
	{

	public:

		static bool buttonUp;
		static SDL_Point point;

		static void update()
		{
			buttonUp = false;
			SDL_GetMouseState(&point.x, &point.y);
		}

	};

	static bool update()
	{
		Mouse::update();
		SDL_Event event;
		// SDL_PollEvent returns number of waiting events (events are stored independently of the funcion call, you just populate `event` variable)
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				return false;
				break;
			case SDL_MOUSEBUTTONUP:
				Mouse::buttonUp = true;
			default:
				break;
			}
		}
		return true;
	}


};