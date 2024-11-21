#include "Game.hpp"

Engine::Engine(const Properties &properties)
	: deltaTime(0), properties(properties), isRunning(false), window(nullptr) 
{
	Engine::Init();
	Engine::GameLoop();
	Engine::Exit();
}

Engine::~Engine() = default;

SDL_Renderer* Engine::renderer = nullptr;
ECS Engine::ecs;

void Engine::Init() 
{
	int flags = properties.fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		printf("Initialized subsystems.\n");
		
		// Create window
		window = SDL_CreateWindow(
			properties.title, (int)properties.windowPos.x, (int)properties.windowPos.y, 
			(int)properties.windowSize.x, (int)properties.windowSize.y, flags
		);

		if (window)
		{
			printf("Window created.\n");
		}

		// Create renderer
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			Color color("#000000");

			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			printf("Renderer created.\n");
		}
		isRunning = true;
	}

	else 
	{
		printf("Failed to initialize SDL");
		isRunning = false;
	}
	
	ecs.Init();

	// Register components
	Init::InitComponents();
	Init::InitSystems();

	SceneManager::AddScene<InitialScene>("initial_scene");
	SceneManager::ChangeScene("initial_scene");
}

bool showFPS = false;

void Engine::Events()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	// Event handler
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;

	case SDL_KEYDOWN:
		// Test animations
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
	ecs.GetSystem<MovementSystem>()->update(ecs, (double)deltaTime / 1000);
	ecs.GetSystem<AABBSystem>()->update(ecs);

	SceneManager::Update(deltaTime);
}

void Engine::Render()
{
	SDL_RenderClear(renderer);

	SceneManager::Draw();
	ecs.GetSystem<SpriteSystem>()->update(ecs, renderer);

	SDL_RenderPresent(renderer);
}

void Engine::GameLoop() 
{
	// Times in milliseconds
	int targetDeltaTime = 1000 / properties.targetFPS;

	Uint32 frameStartTime;
	int frameDrawTime;

	while (Running())
	{
		frameStartTime = SDL_GetTicks();

		Events();
		Update();
		Render();

		frameDrawTime = SDL_GetTicks() - frameStartTime;

		// FPS is at target FPS
		if (targetDeltaTime > frameDrawTime) 
		{
			SDL_Delay(targetDeltaTime - frameDrawTime);
			deltaTime = targetDeltaTime;
		}

		// FPS is less than target FPS
		else 
		{
			deltaTime = frameDrawTime;
		}

		if (showFPS)
		{
			printf("FPS: %f | Entities: %zu\n", ((float)targetDeltaTime / (float)deltaTime) * properties.targetFPS, ecs.GetEntityCount());
			showFPS = false;
		}		
	}
}

void Engine::Exit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	printf("Game exited\n");
}
