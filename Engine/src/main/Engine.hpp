#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "init/Init.hpp"


#include "../ecs/systems/SystemsExport.hpp"
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
	// Constructor
	StarPlatinumEngine(
		const char* title = "Star Platinum Project", 
		int w = 640, int h = 480, bool fullscreen = false
	);

	// Run the engine
	void Run();

private:
	ThreadPool m_pool;

	// Delta time in milliseconds
	double m_deltaTime = 0.0;

	bool events();
	void update();
	void render();

	void exit();
};