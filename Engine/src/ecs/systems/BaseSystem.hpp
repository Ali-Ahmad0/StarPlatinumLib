#pragma once

#include "../definitions.hpp"

// Base class for all systems with a set of entities
struct BaseSystem 
{
	std::array <bool, MAX_ENTITIES> entityRecord;

	virtual void Update(double delta) = 0;
	virtual void OnEntityAdded(EntityID e) = 0; 
	virtual void OnEntityRemoved(EntityID e) = 0;
};
