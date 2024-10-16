#include "Init.hpp"

void Init::InitComponents() 
{
	Engine::GetECS().RegisterComponent<Transform>();
	Engine::GetECS().RegisterComponent<Sprite>();
	Engine::GetECS().RegisterComponent<Movement>();
	Engine::GetECS().RegisterComponent<AABB>();
}

void Init::InitSystems() 
{
	// Sprite System
	auto spriteSystem = Engine::GetECS().RegisterSystem<SpriteSystem>();

	Engine::GetECS().AddComponentToSystem<Transform, SpriteSystem>();
	Engine::GetECS().AddComponentToSystem<Sprite, SpriteSystem>();

	// Movemenet system
	auto movemenetSystem = Engine::GetECS().RegisterSystem<MovementSystem>();

	Engine::GetECS().AddComponentToSystem<Transform, MovementSystem>();
	Engine::GetECS().AddComponentToSystem<Movement, MovementSystem>();

	// AABB system
	auto aabbSystem = Engine::GetECS().RegisterSystem<AABBSystem>();

	Engine::GetECS().AddComponentToSystem<Transform, AABBSystem>();
	Engine::GetECS().AddComponentToSystem<AABB, AABBSystem>();
}