#include "Init.hpp"

void Init::InitComponents() 
{
	ECS::RegisterComponent<Transform>();
	ECS::RegisterComponent<Sprite>();
	ECS::RegisterComponent<Movement>();
	ECS::RegisterComponent<AABB>();
}

void Init::InitSystems() 
{
	// Sprite System
	auto spriteSystem = ECS::RegisterSystem<SpriteSystem>();

	ECS::AddComponentToSystem<Transform, SpriteSystem>();
	ECS::AddComponentToSystem<Sprite, SpriteSystem>();

	// Movemenet system
	auto movemenetSystem = ECS::RegisterSystem<MovementSystem>();

	ECS::AddComponentToSystem<Transform, MovementSystem>();
	ECS::AddComponentToSystem<Movement, MovementSystem>();

	// AABB system
	auto aabbSystem = ECS::RegisterSystem<CollisionSystem>();

	ECS::AddComponentToSystem<Transform, CollisionSystem>();
	ECS::AddComponentToSystem<AABB, CollisionSystem>();
}