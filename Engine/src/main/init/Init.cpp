#include "Init.hpp"

void Init::InitComponents() 
{
	ECS::RegisterComponent<Transform>();
	ECS::RegisterComponent<Sprite>();
	ECS::RegisterComponent<Movement>();
	ECS::RegisterComponent<AABB>();
	ECS::RegisterComponent<Collider>();
}

void Init::InitSystems() 
{
	// Sprite System
	ECS::RegisterSystem<SpriteSystem>();

	ECS::AddComponentToSystem<Transform, SpriteSystem>();
	ECS::AddComponentToSystem<Sprite, SpriteSystem>();

	// Movemenet system
	ECS::RegisterSystem<MovementSystem>();

	ECS::AddComponentToSystem<Transform, MovementSystem>();
	ECS::AddComponentToSystem<Movement, MovementSystem>();

	// Collision system
	ECS::RegisterSystem<CollisionSystem>();

	ECS::AddComponentToSystem<Transform, CollisionSystem>();
	ECS::AddComponentToSystem<AABB, CollisionSystem>();
	//ECS::AddComponentToSystem<Collider, CollisionSystem>();
}