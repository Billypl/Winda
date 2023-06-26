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
	SDL_Rect windowRect = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Window::WIDTH, Window::HEIGHT };
	Game game("Elevator", windowRect);
	while (game.isRunning)
	{
		game.handleEvents();
		game.update();
		game.render();
	}
	game.clean();

	return 0;
}