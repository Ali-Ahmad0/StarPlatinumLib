#include "Engine.hpp"
#include <Windows.h>

SPLib::SPLib(const char* title, int w, int h, bool fullscreen) : delta(0), substeps(12)
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	// Initialize the engine viewport
	if (!ViewPort::Init(title, w, h, fullscreen)) exit();

	// Initialize ECS related stuff
	ECS::Init();
	printf("[INFO]: ECS initialized\n");

	Init::InitComponents();
	printf("[INFO]: Engine components initialized\n");

	Init::InitSystems();
	printf("[INFO]: Engine systems initialized\n");
}

//bool showFPS = false;

bool SPLib::events()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	// Test animations
	switch (event.type)
	{

	case SDL_QUIT:
		return false;
		break;
	
	//case SDL_KEYDOWN:
	//	switch (event.key.keysym.sym)
	//	{
	//	// Show FPS
	//	case SDLK_TAB:
	//		showFPS = true;
	//		break;

	//	default:
	//		break;
	//	}
	//	break;

	default:
		SceneManager::Events(event);
		break;
	}

	return true;
}

void SPLib::update() {
	SceneManager::Update(delta);

	ECS::GetSystem<MovementSystem>()->update(delta);
	ECS::GetSystem<CollisionSystem>()->update();
}

void SPLib::render() 
{
	ECS::GetSystem<SpriteSystem>()->update();
}

void SPLib::Run() 
{
	// Times in milliseconds
	uint32_t targetDelta = 1000 / 60;

	uint32_t frameStartTime;
	uint32_t frameDrawTime;

	printf("[INFO]: Starting update loop...\n");

	while (true)
	{
		frameStartTime = SDL_GetTicks();

		SDL_RenderClear(ViewPort::GetRenderer());
		
		// Events, update and rendering
		if (!events()) break;
		render();
		update();
		SDL_RenderPresent(ViewPort::GetRenderer());

		frameDrawTime = SDL_GetTicks() - frameStartTime;

		// FPS is at target FPS
		if (targetDelta > frameDrawTime) 
		{
			SDL_Delay(targetDelta - frameDrawTime);
			delta = (double)targetDelta / 1000;
		}

		// FPS is less than target FPS
		else 
		{
			delta = (double)frameDrawTime / 1000;
		}

		//if (showFPS)
		//{
		//	printf("FPS: %f | Entities: %zu\n", targetDelta / (delta * 1000) * 60, ECS::GetEntityCount());
		//	showFPS = false;
		//}
	}

	// Close when exiting game loop
	exit();
}

void SPLib::exit()
{
	printf("[INFO]: Exiting...\n");
	
	ViewPort::Exit(); SDL_Quit();

	printf("[INFO]: Game exited\n");
}

