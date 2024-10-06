#include "Init.hpp"

void Init::InitComponents() 
{
	Engine::GetECS().RegisterComponent<Transform>();
	Engine::GetECS().RegisterComponent<Sprite>();
	Engine::GetECS().RegisterComponent<AABB>();
}

void Init::InitSystems() 
{
	// Sprite System
	auto spriteSystem = Engine::GetECS().RegisterSystem<SpriteSystem>();

	Engine::GetECS().AddComponentToSystem<Transform, SpriteSystem>();
	Engine::GetECS().AddComponentToSystem<Sprite, SpriteSystem>();

	// AABB system
	auto aabbSystem = Engine::GetECS().RegisterSystem<AABBSystem>();

	Engine::GetECS().AddComponentToSystem<Transform, AABBSystem>();
	Engine::GetECS().AddComponentToSystem<AABB, AABBSystem>();
}