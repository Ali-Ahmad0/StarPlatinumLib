#pragma once

#include "../definitions.hpp"

class EntityManager 
{
public:
	// Initialize all available entity IDs
	void Init();

	// Returns whether entity exists or not
	bool IsActive(EntityID entity);

	// Return an available entity ID
	EntityID CreateEntity();
	
	// Make entity ID available again
	void DeleteEntity(EntityID entity);
	
	// Returns number of entities
	size_t GetEntityCount();

	// Update the signature of an entity
	void SetSignature(EntityID entity, Signature signature);
	
	// Return the signature of an entity
	Signature GetSignature(EntityID entity);

private:
	// Queue of available entity IDs
	std::queue<EntityID> availableEntities{};

	// An array that stores whether an entity is active or not
	std::array<bool, MAX_ENTITIES> entityStatus{};

	// Array of signatures for each entity
	// Array index represents entity ID
	std::array<Signature, MAX_ENTITIES> signatures{};

	// Total entities
	size_t count = 0;
};
