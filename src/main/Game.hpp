#ifndef GAME_HPP
#define GAME_HPP

#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../common/Utils.hpp"

struct Properties
{

	const char* title;

	int targetFPS;

	Vector2 windowPos;
	Vector2 windowSize;

	bool fullscreen;

	Properties(const char* title, int targetFPS = 60, Vector2 windowPos = Vector2(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED),  
		Vector2 windowSize = Vector2(640, 480), bool fullscreen = false) 
		: title(title), targetFPS(targetFPS), windowPos(windowPos), windowSize(windowSize), fullscreen(fullscreen) {}
};

class Game {
public:
	// Constructor and destructor
	Game(Properties properties);
	~Game();

	void Init();

	void Events();
	void Update();
	void Render();

	void GameLoop();

	void Exit();

	bool Running() { return isRunning; }

	// Get delta time in miliseconds
	int GetDeltaTimeMiliSeconds()
	{
		return deltaTime;
	}

	// Get delta time in seconds
	double GetDeltaTimeSeconds()
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

