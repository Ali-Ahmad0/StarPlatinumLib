#pragma once
#include "../../ecs/ECS.hpp"
#include "../../ecs/systems/System.hpp"

// Includes code for registration of built in components and systems
// Purpose is to reduce code in the init function of the game class

class Init 
{
public:
	// Register all built in components
	static void InitComponents(ECS& ecs);

	// Register all built in systems and set their signatures
	static void InitSystems(ECS& ecs);
};

