#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "../defintions.hpp"

class EntityManager 
{
public:
	void Init();

	bool IsActive(EntityID entity);

	EntityID CreateEntity();
	void DeleteEntity(EntityID entity);
	
	std::vector<EntityID> GetAllEntities();
	size_t GetEntityCount();

	void SetSignature(EntityID entity, Signature signature);
	Signature GetSignature(EntityID entity);

private:
	// Queue of available entity IDs
	std::queue<EntityID> availableEntities;

	// Active entity list to keep track of all active entities
	std::vector<EntityID> activeEntityList;

	// A map that stores whether an entity is active or not
	std::unordered_map<EntityID, bool> entityStatus{};

	// Array of signatures for each entity
	// Array index represents entity ID
	std::array<Signature, MAX_ENTITIES> signatures{};

	// Total entities
	size_t count = 0;
};

#endif // !ENTITY_MANAGER_HPP
