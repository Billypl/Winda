#include "SDL.h"
#include "SDL_image.h"
#include "string"
#include "Window.h"
#include "Game.h"

using namespace std;

int main(int argc, char* args[])
{
	Game game;
	game.init("title", Window::createRect(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Window::WIDTH, Window::HEIGHT), false);

	while (game.isRunning)
	{
		game.handleEvents();
		game.update();
		game.render();
	}
	game.clean();
	SDL_Delay(3000);

	return 0;
}