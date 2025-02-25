#include "Game.hpp"
#include <thread>

SDL_Renderer* StarPlatinumEngine::renderer = nullptr;

StarPlatinumEngine::StarPlatinumEngine(const char* title, int w, int h, bool fullscreen, const Vector2& position)
	: delta(0), window(nullptr) 
{
	int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		printf("[INFO]: Initialized subsystems\n");

		// Create window
		window = SDL_CreateWindow(
			title, (int)position.x, (int)position.y, w, h, flags
		);

		if (window == nullptr)
		{
			fprintf(stderr, "[ERROR]: Unable to create SDL window, exiting...\n");
			exit();
		}

		printf("[INFO]: Window created\n");


		// Create renderer
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer == nullptr)
		{
			fprintf(stderr, "[ERROR]: Unable to create SDL renderer, exiting...\n");
			exit();
		}

		printf("[INFO]: Renderer created\n");
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}

	else
	{
		fprintf(stderr, "[ERROR] Failed to initialize SDL, exiting...\n");
		exit();
	}


	// Initialize ECS related stuff
	ECS::Init();
	printf("[INFO]: ECS initialized\n");

	Init::InitComponents();
	printf("[INFO]: Engine components initialized\n");

	Init::InitSystems();
	printf("[INFO]: Engine systems initialized\n");
}

StarPlatinumEngine::~StarPlatinumEngine() = default;

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
		break;
	}


	SceneManager::Events(event);
	return true;
}

void StarPlatinumEngine::update()
{
	ECS::GetSystem<MovementSystem>()->update(delta);
	ECS::GetSystem<CollisionSystem>()->update();

	SceneManager::Update(delta);
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

		SDL_RenderClear(renderer);
		
		if (!events())
		{
			break;
		}

		std::thread renderThread(&StarPlatinumEngine::render, this);
		std::thread updateThread(&StarPlatinumEngine::update, this);
		
		renderThread.join(); 
		updateThread.join();

		SDL_RenderPresent(renderer);

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

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	printf("[INFO]: Game exited\n");
}

