#include "Game.hpp"
#include <Windows.h>

SDL_Renderer* StarPlatinumEngine::Renderer = nullptr;

StarPlatinumEngine::StarPlatinumEngine(const char* title, int w, int h, bool fullscreen, const Vector2& position)
	: delta(0), window(nullptr) 
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

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
		Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (Renderer == nullptr)
		{
			fprintf(stderr, "[ERROR]: Unable to create SDL renderer, exiting...\n");
			exit();
		}

		printf("[INFO]: Renderer created\n");
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
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

		SDL_RenderClear(Renderer);
		
		// Events, update and rendering
		if (!events()) break;
		update();
		render();
		SDL_RenderPresent(Renderer);

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
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();

	printf("[INFO]: Game exited\n");
}

