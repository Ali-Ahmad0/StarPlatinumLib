#pragma once

#include "Definitions.hpp"

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

	// Make all entity IDs available
	void ClearEntites();

	// Returns the number of entities
	uint16_t GetEntityCount();

	// Update the signature of an entity
	void SetSignature(EntityID entity, Signature signature);
	
	// Return the signature of an entity
	Signature GetSignature(EntityID entity);

private:
	// Queue of available entity IDs
	std::queue<EntityID> m_availableEntities{};

	// Active/inactive entity status 
	std::array<bool, MAX_ENTITIES> m_entityStatus{};

	// Bitset signatures for entities
	std::array<Signature, MAX_ENTITIES> m_signatures{};

	// Total entities
	uint16_t m_entityCount = 0;
};
