#pragma once
#include <SDL.h>
#include <iostream>

class Mouse
{

public:

	static SDL_Rect point;

	static void update()
	{
		SDL_GetMouseState(&point.x, &point.y);
		std::cout << point.x << " : " << point.y << std::endl;
	}

};