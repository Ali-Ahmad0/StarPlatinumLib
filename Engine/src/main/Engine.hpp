#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "init/Init.hpp"

#include "core/ViewPort.hpp"

#include "../ecs/systems/System.hpp"
#include "../ecs/ECS.hpp"

#include "../tilemap/Tilemap.hpp"

#include "../scene/SceneManager.hpp"

#include "../texture/TextureManager.hpp"
#include "../common/Utils.hpp"

#include "../camera/Camera.hpp"

#include "../threads/Threads.hpp"
 
class StarPlatinumEngine 
{
public:
	// Constructor and destructor
	StarPlatinumEngine(const char* title = "Star Platinum Project", int w = 640, int h = 480, bool fullscreen = false, 
		const Vector2& position = Vector2(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED));

	// Run the engine
	void Run();
	//static SDL_Renderer* Renderer;

private:

	//SDL_Window* window;
	ThreadPool pool;

	// Delta time in milliseconds
	double delta;

	bool events();
	void update();
	void render();

	void exit();
};