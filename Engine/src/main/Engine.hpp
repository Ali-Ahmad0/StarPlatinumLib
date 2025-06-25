#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "init/Init.hpp"


#include "../ecs/systems/System.hpp"
#include "../ecs/ECS.hpp"

#include "../tilemap/Tilemap.hpp"

#include "../scene/SceneManager.hpp"

#include "../common/Utils.hpp"
#include "../texture/TextureManager.hpp"
#include "../viewport/ViewPort.hpp"

#include "../camera/Camera.hpp"

#include "../threads/Threads.hpp"
 
class StarPlatinumEngine 
{
public:
	// Constructor and destructor
	StarPlatinumEngine(const char* title = "Star Platinum Project", 
		int w = 640, int h = 480, bool fullscreen = false);

	// Run the engine
	void Run();

private:
	ThreadPool pool;

	// Delta time in milliseconds
	double delta;

	bool events();
	void update();
	void render();

	void exit();
};