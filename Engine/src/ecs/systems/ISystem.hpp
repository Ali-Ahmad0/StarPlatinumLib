#pragma once

#include "../Definitions.hpp"
#include "../../threads/Threads.hpp"

// Base class for all systems with a set of entities
struct ISystem 
{
	std::array <bool, MAX_ENTITIES> entityRecord;

	virtual void Update(double delta) = 0;
	virtual void OnEntityAdded(EntityID e) = 0; 
	virtual void OnEntityRemoved(EntityID e) = 0;
	virtual void OnAllEntitiesRemoved() = 0;

	void SetThreadPool(ThreadPool* pool) { m_pool = pool; }

protected:
	// Reference to engine thread pool
	ThreadPool* m_pool = nullptr;
};
