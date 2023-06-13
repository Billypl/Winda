#pragma once
#include <string>
#include "Window.h"
#include "SDL.h"
#include <iostream>

using namespace std;

class Game
{

	SDL_Rect imgDestR = Window::createRect(0, 0, 128, 128);


public:
	
	void init(const string& title, SDL_Rect screenR, bool fullscreen)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		Window::init(title, screenR, fullscreen);
	}
	void handleEvents()
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;

		default:
			break;
		}

	}
	void update()
	{
		frames++;
		imgDestR.x++;
		cout << frames << endl;
	}
	
	void render()
	{
		SDL_RenderClear(Window::renderer);

		SDL_Texture* s = Window::loadIMG("assets/Harnold.jpg");
		SDL_RenderCopy(Window::renderer, s, NULL, &imgDestR);

		SDL_RenderPresent(Window::renderer);

	}

	void clean()
	{
		SDL_DestroyWindow(Window::window);
		SDL_DestroyRenderer(Window::renderer);
		SDL_Quit();
	}

	bool isRunning = true;
	int frames = 0;

};
