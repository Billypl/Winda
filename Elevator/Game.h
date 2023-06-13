#pragma once
#include <string>
#include "Window.h"
#include "SDL.h"
#include <iostream>
#include "Mouse.h"
#include "GameObject.h"

using namespace std;

class Game
{

	SDL_Texture* HarnoldIMG;
	GameObject Harnold;

public:
	
	Game(const string& title, SDL_Rect screenR, bool fullscreen)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		Window::init(title, screenR, fullscreen);
		HarnoldIMG = Window::loadIMG("assets/Harnold.jpg");
		Harnold.set(HarnoldIMG, Window::createRect(0, 0, 128, 128));
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
		Harnold.rect.x++;
		Mouse::update();
	}
	
	void render()
	{
		SDL_RenderClear(Window::renderer);
		Harnold.render();
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
