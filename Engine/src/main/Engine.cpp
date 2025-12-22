#include "Engine.hpp"
#include <Windows.h>
#include "../text/TextManager.hpp"

StarPlatinumEngine::StarPlatinumEngine(const char* title, int w, int h, bool fullscreen)
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	// Initialize the engine viewport
	if (!ViewPort::Init(title, w, h, fullscreen)) exit();
	SDL_SetRenderDrawBlendMode(ViewPort::GetRenderer(), SDL_BLENDMODE_BLEND);

	// Initialize SDL_TTF
	if (TTF_Init() == -1) 
	{
		fprintf(stderr, "[ERROR]: Failed to initialize SDL_ttf: %s\n", TTF_GetError());
		exit();
	}
	printf("[INFO]: SDL_ttf initialized successfully\n");

	// Initialize ECS
	ECS::Init();
	printf("[INFO]: ECS initialized\n");

	// Initialize components
	EngineInitializer::InitComponents();
	printf("[INFO]: Engine components initialized\n");

	// Initialize systems
	EngineInitializer::InitSystems();
	ECS::GetSystem<CollisionSystem>()->SetThreadPool(&m_pool);
	ECS::GetSystem<MovementSystem>()->SetThreadPool(&m_pool);
	ECS::GetSystem<SpriteSystem>()->SetThreadPool(&m_pool);
	ECS::GetSystem<VIntegrationSystem>()->SetThreadPool(&m_pool);
	printf("[INFO]: Engine systems initialized\n");
}

bool StarPlatinumEngine::events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) 
	{	
		switch (event.type)
		{

		case SDL_QUIT:
			return false;
			break;

		default:
			SceneManager::Events(event);
			break;
		}
	}

	return true;
}

void StarPlatinumEngine::update() {
	// Scene updates
	auto sceneTask = m_pool.AddTask([this]() {
		SceneManager::Update(m_deltaTime);
	});
	sceneTask.wait();
	
	// Physics updates
	auto physicsTask = m_pool.AddTask([this]() {
		ECS::GetSystem<MovementSystem>()->Update(m_deltaTime);
		ECS::GetSystem<VIntegrationSystem>()->Update(m_deltaTime);
	});
	physicsTask.wait();

	// Collision updates
	auto collisionTask = m_pool.AddTask([this]() {
		ECS::GetSystem<CollisionSystem>()->Update();
	});
	collisionTask.wait();
}

void StarPlatinumEngine::render() 
{
	// Rendering updates
	auto renderingTask = m_pool.AddTask([this]() {
		ECS::GetSystem<SpriteSystem>()->Update();
	});
	renderingTask.wait();
}

void StarPlatinumEngine::exit()
{
	printf("[INFO]: Exiting...\n");

	TextManager::Cleanup();
	TextureManager::Cleanup();

	ViewPort::Exit();
	SDL_Quit();

	printf("[INFO]: Game exited\n");
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
		render();
		update();
		SDL_RenderPresent(ViewPort::GetRenderer());

		frameDrawTime = SDL_GetTicks() - frameStartTime;

		// FPS is at target FPS
		if (targetDelta > frameDrawTime) 
		{
			SDL_Delay(targetDelta - frameDrawTime);
			m_deltaTime = (double)targetDelta / 1000;
		}

		// FPS is less than target FPS
		else 
		{
			m_deltaTime = (double)frameDrawTime / 1000;
		}
	}

	// Close when exiting game loop
	exit();
}