#include "Game.hpp"

Game::Game(const Properties &properties)
	: deltaTime(0), properties(properties), isRunning(false), window(nullptr), renderer(nullptr) {}
Game::~Game() = default;

ECS ecs;
EntityID player;

SDL_Texture* playerTexture;
SDL_Texture* playerPreview;

void Game::Init() 
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

	playerTexture = TextureManager::LoadTexture("assets/character.png", renderer);
	playerPreview = TextureManager::LoadTexture("assets/character_preview.png", renderer);
	
	ecs.Init();

	// Register components
	Init::InitComponents(ecs);

	// Create player entity
	player = ecs.CreateEntity();

	// Register systems and set their signatures
	Init::InitSystems(ecs);

	// Add components
	ecs.AddComponent(player, Transform(Vector2(288, 172), 4));
	ecs.AddComponent(player, Sprite(playerTexture, 3, 4, 6));
}

bool spawnEntities = false;
bool deleteEntities = false;
bool showFPS = false;

void Game::Events()
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
		case SDLK_DOWN:
			ecs.GetComponent<Sprite>(player)->v_frame = 0;
			break;

		case SDLK_UP:
			ecs.GetComponent<Sprite>(player)->v_frame = 1;
			break;

		case SDLK_LEFT:
			ecs.GetComponent<Sprite>(player)->v_frame = 2;
			break;

		case SDLK_RIGHT:
			ecs.GetComponent<Sprite>(player)->v_frame = 3;
			break;

		// Show FPS
		case SDLK_TAB:
			showFPS = true;
			break;

		// Spawn 100 entites
		case SDLK_RETURN:
			spawnEntities = true;
			break;

		default: ;
		}

		break;
	default:
		break;
	}
}

void Game::Update()
{
	// Testing entity spawning
	if (spawnEntities)
	{
		// Seed the random number generator
		srand(static_cast<unsigned int>(time(nullptr)));

		try 
		{
			for (int i = 0; i < 500; i++)
			{
				EntityID entity = ecs.CreateEntity();

				// Set random positions within the screen bounds (640 x 480)
				float randomX = static_cast<float>(rand() % 640);
				float randomY = static_cast<float>(rand() % 480);

				// Assign components to the entity
				ecs.AddComponent(entity, Transform(Vector2(randomX, randomY), 2));
				ecs.AddComponent(entity, Sprite(playerPreview));

			}
		}

		catch (const std::runtime_error& e) 
		{
			printf("%s\n", e.what());
		}

		// Reset the flag
		spawnEntities = false;
	}

	ecs.GetSystem<SpriteSystem>()->update(ecs);
}

void Game::Render()
{
	SDL_RenderClear(renderer);

	ecs.GetSystem<SpriteSystem>()->render(ecs, renderer);

	SDL_RenderPresent(renderer);
}

void Game::GameLoop() 
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

void Game::Exit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	printf("Game exited\n");
}
