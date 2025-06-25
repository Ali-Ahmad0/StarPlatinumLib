#include "Engine.hpp"
#include <Windows.h>

StarPlatinumEngine::StarPlatinumEngine(const char* title, int w, int h, bool fullscreen) : delta(0)
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

bool showFPS = false;

bool StarPlatinumEngine::events()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	// Test animations
	switch (event.type)
	{

	case SDL_QUIT:
		return false;
		break;
	
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		// Show FPS
		case SDLK_TAB:
			showFPS = true;
			break;

		default:
			break;
		}
		break;

	default:
		SceneManager::Events(event);
		break;
	}

	return true;
}

void StarPlatinumEngine::update() {
	// Parallelized systems and scene update logic
	auto movementFuture = pool.AddTask([this]() {
		ECS::GetSystem<MovementSystem>()->update(delta);
	});
	
	auto sceneUpdateFuture = pool.AddTask([this]() {
		SceneManager::Update(delta);
	});

	movementFuture.wait();
	sceneUpdateFuture.wait();

	// Handle collisions at the end on main thread
	ECS::GetSystem<CollisionSystem>()->update();
}

void StarPlatinumEngine::render() 
{
	ECS::GetSystem<SpriteSystem>()->update();
}

void StarPlatinumEngine::Run() 
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
		update();
		render();
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

		if (showFPS)
		{
			printf("FPS: %f | Entities: %zu\n", targetDelta / (delta * 1000) * 60, ECS::GetEntityCount());
			showFPS = false;
		}		
	}

	// Close when exiting game loop
	exit();
}

void StarPlatinumEngine::exit()
{
	printf("[INFO]: Exiting...\n");
	
	ViewPort::Exit(); SDL_Quit();

	printf("[INFO]: Game exited\n");
}

