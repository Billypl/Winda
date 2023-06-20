#pragma once
#include <string>
#include "Window.h"
#include "SDL.h"
#include <iostream>
#include "Button.h"
#include "Input.h"
#include "GameObject.h"
#include "Elevator.h"

using namespace std;

class Game
{

	vector<Button> buttons;

public:
	
	bool isRunning = true;
	int frames = 0;

	Game(const string& title, SDL_Rect screenR, bool fullscreen)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		Window::init(title, screenR, fullscreen);

		generateButtons();
	}

	void generateButtons()
	{
		const int btnSize = 36;
		const int btnColumnStartingX = 20;
		int btnColumnY = Elevator::get().rect.h + Elevator::get().startingRect.y - btnSize;
		int btnColumnX = btnColumnStartingX;
		for (int i = 0; i < Elevator::get().NUMBER_OF_FLOORS; i++)
		{
			for (int j = 0; j < Elevator::get().NUMBER_OF_FLOORS; j++)
			{
				if (i != j)
				{
					Button btn;
					btnColumnX += btn.rect.w + 1;
					btn.setPos(btnColumnX, btnColumnY);
					btn.rect.h = btnSize;
					btn.rect.w = btnSize;
					btn.srcFloor = Elevator::get().NUMBER_OF_FLOORS - i - 1;
					btn.dstFloor = Elevator::get().NUMBER_OF_FLOORS - j - 1;
					btn.text = to_string(Elevator::get().NUMBER_OF_FLOORS - j - 1);
					buttons.push_back(btn);
				}
			}
			btnColumnY += Elevator::get().startingRect.h;
			btnColumnX = btnColumnStartingX;
		}
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
		Elevator::get().update();
		for (Button &button : buttons)
		{
			button.update();
		}
	}
	
	void render()
	{
		SDL_RenderClear(Window::renderer);
		Elevator::get().render();

		for (Button &button : buttons)
		{
			button.render();
		}
		SDL_RenderPresent(Window::renderer);
	}

	void clean()
	{
		SDL_DestroyWindow(Window::window);
		SDL_DestroyRenderer(Window::renderer);
		SDL_Quit();
	}

};
