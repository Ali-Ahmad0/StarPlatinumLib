#include "Init.hpp"
#include "../../ecs/systems/System.hpp"

void Init::InitComponents(ECS& ecs) 
{
	ecs.RegisterComponent<Transform>();
	ecs.RegisterComponent<Sprite>();
}

void Init::InitSystems(ECS& ecs) 
{
	// Sprite System
	auto spriteSystem = ecs.RegisterSystem<SpriteSystem>();

	Signature spriteSignature;
	spriteSignature.set(ecs.GetComponentID<Transform>(), true);
	spriteSignature.set(ecs.GetComponentID<Sprite>(), true);

	ecs.SetSystemSignature<SpriteSystem>(spriteSignature);
}