#pragma once
#include "../../ecs/systems/System.hpp"
#include "../Game.hpp"

// Includes code for registration of built in components and systems
// Purpose is to reduce code in the init function of the game class

class Init 
{
public:
	// Register all built in components
	static void InitComponents();

	// Register all built in systems and set their signatures
	static void InitSystems();
};

