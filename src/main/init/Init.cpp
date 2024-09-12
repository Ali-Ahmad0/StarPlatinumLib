#include "Init.hpp"

void Init::InitComponents(ECS& ecs) 
{
	ecs.RegisterComponent<Transform>();
	ecs.RegisterComponent<Sprite>();
}

void Init::InitSystems(ECS& ecs) 
{
	// Sprite System
	auto spriteSystem = ecs.RegisterSystem<SpriteSystem>();

	ecs.AddComponentToSystem<Transform, SpriteSystem>();
	ecs.AddComponentToSystem<Sprite, SpriteSystem>();
}