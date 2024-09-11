#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#include "init/Init.hpp"
#include "../ecs/ECS.hpp"

#include "../texture/TextureManager.hpp"
#include "../ecs/systems/System.hpp"

#include "../common/Utils.hpp"

struct Properties
{
	const char* title;

	int targetFPS;

	Vector2 windowPos;
	Vector2 windowSize;

	bool fullscreen;

	explicit Properties(const char* title, int targetFPS = 60, Vector2 windowPos = Vector2(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED),
		Vector2 windowSize = Vector2(640, 480), bool fullscreen = false) 
		: title(title), targetFPS(targetFPS), windowPos(windowPos), windowSize(windowSize), fullscreen(fullscreen) {}
};

class Game {
public:
	// Constructor and destructor
	explicit Game(const Properties &properties);
	~Game();

	void Init();

	void Events();
	void Update();
	void Render();

	void GameLoop();

	void Exit();

	bool Running() const { return isRunning; }

	// Get delta time in miliseconds
	int GetDeltaTimeMilliSeconds() const
	{
		return deltaTime;
	}

	// Get delta time in seconds
	double GetDeltaTimeSeconds() const
	{
		return (double)deltaTime / 1000;
	}

private:
	bool isRunning;

	// Delta time in milliseconds
	int deltaTime;

	SDL_Window* window;
	SDL_Renderer* renderer;

	Properties properties;
};


