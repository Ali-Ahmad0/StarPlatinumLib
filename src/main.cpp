#include "main/Game.hpp"
#undef main

Game* game = nullptr;

int main() 
{
	game = new Game();
	game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, false);
	
	game->gameLoop();

	game->exit();

	return 0;
}


