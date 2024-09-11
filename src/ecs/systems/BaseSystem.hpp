#pragma once

#include "../defintions.hpp"

// Base struct for all systems with a set of entities
struct BaseSystem 
{
	std::set<EntityID> entities;
};


