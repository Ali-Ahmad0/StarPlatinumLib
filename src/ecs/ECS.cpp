#include "ECS.hpp"

void ECS::Init() 
{
    for (EntityID e = 0; e < MAX_ENTITIES; e++)
    {
        availableEntities.push(e);
    }
}

EntityID ECS::CreateEntity() 
{
    EntityID entity = availableEntities.front();
    availableEntities.pop();

    activeEntityList.push_back(entity);
    return entity;
}

void ECS::DeleteEntity(EntityID entity) 
{
    // Remove entity from active entity list
    auto it = std::find(activeEntityList.begin(), activeEntityList.end(), entity);

    if (it != activeEntityList.end())
    {
        availableEntities.push(entity);
        activeEntityList.erase(it);
    }

    // Remove components associated with the entity
    for (auto& pair : componentPools)
    {
        pair.second->EntityDestroyed(entity);
    }
}

size_t ECS::GetEntityCount() 
{
    return entityCount;
}