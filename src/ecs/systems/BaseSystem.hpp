#pragma once

#include "../defintions.hpp"

// Base class for all systems with a set of entities
struct BaseSystem 
{
	std::vector<EntityID> entities;
	std::array<bool, MAX_ENTITIES> entityRecord;
};
