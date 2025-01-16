#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "init/Init.hpp"
#include "../ecs/systems/System.hpp"
#include "../ecs/ECS.hpp"

#include "../tilemap/Tilemap.hpp"

#include "../scene/SceneManager.hpp"
#include "../scene/InitialScene.hpp"

#include "../texture/TextureManager.hpp"
#include "../common/Utils.hpp"

#include "../camera/Camera.hpp"

struct Properties
{
	const char* title;

	bool fullscreen;

	int targetFPS;

	Vector2 windowPos;
	Vector2 windowSize;


	Properties(const char* title, int targetFPS = 60, Vector2 windowSize = Vector2(640, 480), bool fullscreen = false, 
		Vector2 windowPos = Vector2(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED)) 
		: title(title), targetFPS(targetFPS), windowSize(windowSize), fullscreen(fullscreen), windowPos(windowPos) {}
};

class Engine 
{
public:
	// Constructor and destructor
	Engine(const Properties& properties);
	~Engine();

	void Init();

	void Events();
	void Update();

	void GameLoop();

	void Exit();

	// Engine property getters
	bool Running() 
	{ 
		return isRunning; 
	}
	
	static SDL_Renderer* GetRenderer() 
	{ 
		return renderer; 
	}

private:
	bool isRunning;

	// Delta time in milliseconds
	double delta;

	SDL_Window* window;
	static SDL_Renderer* renderer;

	Properties properties;

};


