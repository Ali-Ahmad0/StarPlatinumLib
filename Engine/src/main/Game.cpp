#include "Game.hpp"
#include <thread>

SDL_Renderer* Engine::renderer = nullptr;

Engine::Engine(const Properties &properties)
	: delta(0), properties(properties), isRunning(false), window(nullptr) {}

Engine::~Engine() = default;

void Engine::Init() 
{
	int flags = properties.fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		printf("[INFO]: Initialized subsystems\n");
		
		// Create window
		window = SDL_CreateWindow(
			properties.title, (int)properties.windowPos.x, (int)properties.windowPos.y, 
			(int)properties.windowSize.x, (int)properties.windowSize.y, flags
		);

		if (window)
		{
			printf("[INFO]: Window created\n");
		}

		// Create renderer
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			Color color("#000000");

			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			printf("[INFO]: Renderer created\n");
		}
		isRunning = true;
	}

	else 
	{
		fprintf(stderr, "[ERROR] Failed to initialize SDL, exiting...\n");
		isRunning = false;
	}
	

	// Initialize ECS related stuff
	ECS::Init();
	printf("[INFO]: ECS initialized\n");

	Init::InitComponents();
	printf("[INFO]: Engine components initialized\n");

	Init::InitSystems();
	printf("[INFO]: Engine systems initialized\n");
}

bool showFPS = false;

void Engine::Events()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	// Test animations
	switch (event.type)
	{

	case SDL_QUIT:
		isRunning = false;
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
		break;
	}


	SceneManager::Events(event);
}

void Engine::Update()
{
	ECS::GetSystem<MovementSystem>()->update(delta);
	ECS::GetSystem<CollisionSystem>()->update();

	SceneManager::Update(delta);
}

void Engine::Render() 
{
	ECS::GetSystem<SpriteSystem>()->update();
}

void Engine::GameLoop() 
{
	// Times in milliseconds
	uint32_t targetDeltaTime = 1000 / properties.targetFPS;

	uint32_t frameStartTime;
	uint32_t frameDrawTime;

	printf("[INFO]: Starting update loop...\n");

	while (Running())
	{
		frameStartTime = SDL_GetTicks();

		SDL_RenderClear(renderer);
		
		Events();

		std::thread updateThread(&Engine::Update, this);
		std::thread renderThread(&Engine::Render, this);
		
		updateThread.join();
		renderThread.join();

		SDL_RenderPresent(renderer);

		frameDrawTime = SDL_GetTicks() - frameStartTime;

		// FPS is at target FPS
		if (targetDeltaTime > frameDrawTime) 
		{
			SDL_Delay(targetDeltaTime - frameDrawTime);
			delta = (double)targetDeltaTime / 1000;
		}

		// FPS is less than target FPS
		else 
		{
			delta = (double)frameDrawTime / 1000;
		}

		if (showFPS)
		{
			printf("FPS: %f | Entities: %zu\n", ((float)targetDeltaTime / (float)delta) * properties.targetFPS, ECS::GetEntityCount());
			showFPS = false;
		}		
	}
}

void Engine::Exit()
{
	printf("[INFO]: Exiting...\n");

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	printf("[INFO]: Game exited\n");
}

