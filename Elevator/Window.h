#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <string>
using namespace std;

class Window
{

public:
	
	static const int WIDTH;
	static const int HEIGHT;
	static SDL_Window* window;
	static SDL_Renderer* renderer;

	static void init(const string& title, SDL_Rect screenR, bool fullscreen)
	{
		int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;

		window = SDL_CreateWindow(title.c_str(), screenR.x, screenR.y, screenR.w, screenR.h, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	}

	static SDL_Texture* loadIMG(std::string filename)
	{
		SDL_Surface* tmp = IMG_Load(filename.c_str());
		SDL_Texture* image = SDL_CreateTextureFromSurface(renderer, tmp);
		SDL_FreeSurface(tmp);
		return image;
	}

	static SDL_Rect createRect(int x, int y, int w, int h)
	{
		SDL_Rect tmp = { x, y, w, h };
		return tmp;
	}


};

