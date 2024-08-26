#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "defintions.hpp"

class EntityManager 
{
public:
	void Init();

	EntityID CreateEntity();
	void DeleteEntity(EntityID entity);
	
	std::vector<EntityID> GetAllEntities();
	size_t GetEntityCount();

	void SetSignature(EntityID entity, Signature s) 
	{
		
	}

private:
	// Queue of available entity IDs
	std::queue<EntityID> availableEntities;

	// Active entity list to keep track of all active entities
	std::vector<EntityID> activeEntityList;

	// Total entities
	size_t count = 0;
};

#endif // !ENTITY_MANAGER_HPP
