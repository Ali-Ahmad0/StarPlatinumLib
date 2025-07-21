#pragma once

#include "../definitions.hpp"

// Base class for all systems with a set of entities
struct BaseSystem 
{
	std::array <bool, MAX_ENTITIES> entityRecord;

	virtual void update(double delta) = 0;
	virtual void onEntityAdded(EntityID e) = 0; 
	virtual void onEntityRemoved(EntityID e) = 0;
};
