#pragma once
#include <string>
#include "Window.h"
#include "SDL.h"
#include <iostream>
#include "Button.h"
#include "Input.h"
#include "GameObject.h"

using namespace std;

class Game
{

	SDL_Texture* HarnoldIMG;
	GameObject Harnold;
	GameObject Harnold2;
	Button btn;

public:
	
	Game(const string& title, SDL_Rect screenR, bool fullscreen)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		Window::init(title, screenR, fullscreen);
		HarnoldIMG = Window::loadIMG("assets/Harnold.jpg");
		Harnold.set(HarnoldIMG, Window::createRect(0, 0, 128, 128));
		btn.text = "1";
		btn.setPos(50, 50);
		btn.callback = []() {cout << "Button clicked!\n"; };
	}

	void handleEvents()
	{
		if (Input::update() == false)
		{
			isRunning = false;
		}
	}
	void update()
	{
		frames++;
		Harnold.rect.x++;
		btn.update();
	}
	
	void render()
	{
		SDL_RenderClear(Window::renderer);
		Harnold.render();
		
		btn.render();
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
