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

class Engine 
{
public:
	// Constructor and destructor
	explicit Engine(const Properties &properties);
	~Engine();

	void Init();

	void Events();
	void Update();
	void Render();

	void GameLoop();

	void Exit();

	bool Running() const { return isRunning; }

	static SDL_Renderer* GetRenderer() { return renderer; }
	static ECS& GetECS() { return ecs; }

private:
	bool isRunning;

	// Delta time in milliseconds
	int deltaTime;

	SDL_Window* window;
	static SDL_Renderer* renderer;
	
	static ECS ecs;
	Properties properties;

};


