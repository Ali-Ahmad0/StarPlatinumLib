#pragma once
#include "../Engine.hpp"
#include "../../ecs/systems/SystemsExport.hpp"

// Includes code for registration of built in components and systems
// Purpose is to reduce code in the init function of the game class

class EngineInitializer 
{
public:
	// Register all built in components
	static void InitComponents();

	// Register all built in systems and set their signatures
	static void InitSystems();
};