#pragma once
#include <string>
#include "Window.h"
#include "SDL.h"
#include <iostream>
#include "Button.h"
#include "Input.h"
#include "Elevator.h"

using namespace std;

class Game
{

	vector<Button> buttons;

public:
	
	bool isRunning = true;

	Game(const string& title, SDL_Rect screenR)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		Window::init(title, screenR);

		generateButtons();
	}

	void generateButtons()
	{
		const int BUTTON_SIZE = 36;
		const int BUTTON_COLUMN_STARTING_X = 20;
		const int BUTTON_MARGIN_X = 1;

		int btnColumnY = Elevator::get().elevatorCarRect.h + Elevator::get().startingRect.y - BUTTON_SIZE;
		int btnColumnX = BUTTON_COLUMN_STARTING_X;
		// i - row (starting form the top floor)
		// j - column (printing all of the other possible floors)
		for (int i = Elevator::get().NUMBER_OF_FLOORS - 1; i >= 0; i--)
		{
			for (int j = 0; j < Elevator::get().NUMBER_OF_FLOORS; j++)
			{
				if (i != j) // don't generate buttons to the same floor
				{
					btnColumnX += BUTTON_SIZE + BUTTON_MARGIN_X;
					
					Button btn;
					btn.rect = { btnColumnX, btnColumnY, BUTTON_SIZE, BUTTON_SIZE };
					btn.srcFloor = i;
					btn.dstFloor = j;
					btn.text = to_string(btn.dstFloor);
					buttons.push_back(btn);
				}
			}
			btnColumnY += Elevator::get().startingRect.h;
			btnColumnX = BUTTON_COLUMN_STARTING_X;
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
