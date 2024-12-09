#include "main/Game.hpp"
#undef main

Engine* game = nullptr;

int main() 
{
	game = new Engine(Properties("Game", 60, Vector2(640, 480)));

	// Free memory
	delete game;
	game = nullptr;

	return 0;
}


