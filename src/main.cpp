#include "main/Game.hpp"
#undef main

Game* game = nullptr;

int main() 
{
	game = new Game(Properties("Game"));
	
	game->Init();
	game->GameLoop();
	game->Exit();

	return 0;
}


