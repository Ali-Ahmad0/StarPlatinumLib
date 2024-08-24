#ifndef GAME_HPP
#define GAME_HPP

#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../common/Properties.hpp"
#include "../common/Utils.hpp"

class Game {
public:
	// Constructor and destructor
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void events();
	void update();
	void render();

	void gameLoop();

	void exit();

	bool running() { return isRunning; }

	// Get delta time in miliseconds
	int getDeltaTimeMiliSeconds()
	{
		return deltaTime;
	}

	// Get delta time in seconds
	double getDeltaTimeSeconds()
	{
		return (double)deltaTime / 1000;
	}

private:
	bool isRunning;

	// Delta time in miliseconds
	int deltaTime;

	SDL_Window* window;
	SDL_Renderer* renderer;

	Properties properties;
};

#endif // !GAME_HPP

