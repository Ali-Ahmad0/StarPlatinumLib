#include <vector>

#include "Game.hpp"
#include "../ecs/ECS.hpp"
#include "../texture/TextureManager.hpp"
#include "../ecs//systems/System.hpp"


Game::Game(Properties properties) 
	: deltaTime(0), properties(properties), isRunning(false), window(nullptr), renderer(nullptr) {}
Game::~Game() {}

ECS ecs;
SpriteSystem spriteSystem;

EntityID player;

SDL_Texture* playerTexture;

void Game::init() 
{
	int flags = properties.fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		//printf("Initialized subsystems.\n");
		
		// Create window
		window = SDL_CreateWindow(
			properties.title, (int)properties.windowPos.x, (int)properties.windowPos.y, 
			(int)properties.windowSize.x, (int)properties.windowSize.y, flags
		);

		if (window)
		{
			//printf("Window created.\n");
		}

		// Create renderer
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			Color color("#000000");

			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			//printf("Renderer created.\n");
		}
		isRunning = true;
	}

	else 
	{
		//printf("Failed to initialize SDL");
		isRunning = false;
	}

	playerTexture = SpriteManager::LoadTexture("assets/character.png", renderer);
	
	ecs.Init();

	// Create player entity
	player = ecs.CreateEntity();

	ecs.RegisterComponent<TransformComponent>();
	ecs.RegisterComponent<SpriteComponent>();

	// Add components
	ecs.AddComponent(player, TransformComponent(Vector2(288, 172), 4));
	ecs.AddComponent(player, SpriteComponent(playerTexture, 3, 4, 6));
}

bool spawnEntities = false;
bool deleteEntities = false;
bool showFPS = false;

void Game::events()
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
		//if (event.key.keysym.sym == SDLK_RETURN) // Check if "Enter" key is pressed
		//{
		//	spawnEntities = true; // Set the flag to spawn new entities
		//}

		//if (event.key.keysym.sym == SDLK_DELETE) 
		//{
		//	deleteEntities = true;
		//}

		// Test animations
		switch (event.key.keysym.sym)
		{
		case SDLK_DOWN:
			ecs.GetComponent<SpriteComponent>(player)->v_frame = 0;
			break;

		case SDLK_UP:
			ecs.GetComponent<SpriteComponent>(player)->v_frame = 1;
			break;

		case SDLK_LEFT:
			ecs.GetComponent<SpriteComponent>(player)->v_frame = 2;
			break;

		case SDLK_RIGHT:
			ecs.GetComponent<SpriteComponent>(player)->v_frame = 3;
			break;

		case SDLK_TAB:
			showFPS = true;
			break;
		}

		break;
	default:
		break;
	}
}

void Game::update()
{
	//// Testing entity spawning
	//if (spawnEntities)
	//{
	//	// Seed the random number generator
	//	srand(static_cast<unsigned int>(time(0)));

	//	for (int i = 0; i < 100; i++)
	//	{
	//		EntityID entity = ecs.createEntity();

	//		// Assign components to the entity
	//		
	//		ecs.addComponent(entity, std::make_shared<SpriteComponent>(playerTexture));

	//		// Set random positions within the screen bounds (640 x 480)
	//		float randomX = static_cast<float>(rand() % 640);
	//		float randomY = static_cast<float>(rand() % 480);

	//		ecs.getComponent<TransformComponent>(entity)->position = Vector2(randomX, randomY);
	//		ecs.getComponent<TransformComponent>(entity)->scale = 2;
	//	}

	//	// Reset the flag
	//	spawnEntities = false;
	//}

	//// Testing entity deletion
	//if (deleteEntities)
	//{
	//	size_t firstEntity = ecs.getNextEntity() - ecs.getEntityCount();

	//	for (size_t e = firstEntity; e < firstEntity + 100; e++)
	//	{
	//		ecs.deleteEntity(e);
	//	}

	//	deleteEntities = false;
	//}

	

	spriteSystem.update(ecs);
	
}

void Game::render()
{
	SDL_RenderClear(renderer);

	spriteSystem.render(ecs, renderer);

	SDL_RenderPresent(renderer);

	
}

void Game::gameLoop() 
{
	// Times in miliseconds
	int targetDeltaTime = 1000 / properties.targetFPS;

	Uint32 frameStartTime;
	int frameDrawTime;

	while (running())
	{
		frameStartTime = SDL_GetTicks();

		events();
		update();
		render();

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
			printf("FPS: %f | Entities: %zu\n", ((float)targetDeltaTime / (float)deltaTime) * properties.targetFPS, ecs.GetAllEntities().size());
			showFPS = false;
		}
		
	}
}

void Game::exit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	//printf("Game exited\n");
}
