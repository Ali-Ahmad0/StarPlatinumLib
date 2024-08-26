#include "ECS.hpp"

void EntityManager::Init() 
{
    for (EntityID e = 0; e < MAX_ENTITIES; e++)
    {
        availableEntities.push(e);
    }
}

EntityID EntityManager::CreateEntity() 
{
    EntityID entity = availableEntities.front();
    availableEntities.pop();

    activeEntityList.push_back(entity);
    return entity;
}

void EntityManager::DeleteEntity(EntityID entity) 
{
    // Remove entity from active entity list
    auto it = std::find(activeEntityList.begin(), activeEntityList.end(), entity);

    if (it != activeEntityList.end())
    {
        availableEntities.push(entity);
        activeEntityList.erase(it);
    }

    // Remove components associated with the entity
    //for (auto& pair : componentPools)
    //{
    //    pair.second->EntityDestroyed(entity);
    //}
}

std::vector<EntityID> EntityManager::GetAllEntities()
{
    return activeEntityList;
}

size_t EntityManager::GetEntityCount() 
{
    return count;
}