#include "Init.hpp"

void Init::InitComponents() 
{
	Engine::GetECS().RegisterComponent<Transform>();
	Engine::GetECS().RegisterComponent<Sprite>();
}

void Init::InitSystems() 
{
	// Sprite System
	auto spriteSystem = Engine::GetECS().RegisterSystem<SpriteSystem>();

	Engine::GetECS().AddComponentToSystem<Transform, SpriteSystem>();
	Engine::GetECS().AddComponentToSystem<Sprite, SpriteSystem>();
}