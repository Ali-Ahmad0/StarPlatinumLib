#include "main/Game.hpp"
#undef main

Game* game = nullptr;

int main() 
{
	game = new Game(Properties("Game"));
	
	game->init();
	game->gameLoop();
	game->exit();

	return 0;
}


