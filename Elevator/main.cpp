#include "SDL.h"
#include "SDL_image.h"
#include "string"
#include "Window.h"
#include "Game.h"
#include "Elevator.h"
#include <queue>

using namespace std;




int main(int argc, char* args[])
{

	Game game("Elevator", Window::createRect(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Window::WIDTH, Window::HEIGHT), false);
	while (game.isRunning)
	{
		game.handleEvents();
		game.update();
		game.render();
	}
	game.clean();



	//Elevator &elevator = Elevator::get();
	//elevator.init();
	//elevator.currentFloor = 2;
	////elevator.peopleWaiting.push_back(Person(1, 2));
	////elevator.peopleWaiting.push_back(Person(3, 1));
	////elevator.peopleWaiting.push_back(Person(2, 5));
	////elevator.peopleWaiting.push_back(Person(2, 5));
	//elevator.peopleWaiting.push_back(Person(3, 5));
	//elevator.peopleWaiting.push_back(Person(1, 5));
	//
	//while (true)
	//{
	//	elevator.update();
	//}

	return 0;
}