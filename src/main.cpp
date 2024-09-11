#include "main/Game.hpp"
#undef main

Game* game = nullptr;

int main() 
{
	game = new Game(Properties("Game"));
	
	// Run game
	game->Init();
	game->GameLoop();
	game->Exit();

	// Free memory
	delete game;
	game = nullptr;

	return 0;
}


