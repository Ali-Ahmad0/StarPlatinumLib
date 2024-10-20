#include "main/Game.hpp"
#undef main

Engine* game = nullptr;

int main() 
{
	game = new Engine(Properties("Game"));

	// Free memory
	delete game;
	game = nullptr;

	return 0;
}


