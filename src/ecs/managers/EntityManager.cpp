#include "EntityManager.hpp"

void EntityManager::Init() 
{
    for (EntityID e = 0; e < MAX_ENTITIES; e++)
    {
        availableEntities.push(e);
    }
}

bool EntityManager::IsActive(const EntityID entity)
{
    return entity < MAX_ENTITIES && entityStatus[entity];
}

EntityID EntityManager::CreateEntity() 
{
    if (count < MAX_ENTITIES)
    {
        const EntityID entity = availableEntities.front();
        availableEntities.pop();
        entityStatus[entity] = true;

        count++;
        return entity;
    }

    throw std::runtime_error("[RUNTIME ERROR]: Max entity limit reached");
    
}

void EntityManager::DeleteEntity(EntityID entity)
{
    if (IsActive(entity))
    {
        availableEntities.push(entity);
        entityStatus[entity] = false;
        count--;
    }
    else 
    {
        fprintf(stderr, "[ERROR]: Cannot delete non existent entity\n");
    }
    
}

size_t EntityManager::GetEntityCount()
{
    return count;
}

void EntityManager::SetSignature(EntityID entity, ComponentSignature signature)
{
    if (IsActive(entity)) 
    {
        signatures[entity] = signature;
    }
    else 
    {
        fprintf(stderr, "[ERROR]: Cannot set signature on non existent entity\n");
    }
}

ComponentSignature EntityManager::GetSignature(EntityID entity)
{
    if (IsActive(entity)) 
    {
        return signatures[entity];
    }

    throw std::runtime_error("[RUNTIME ERROR]: Cannot get signature on non existent entity");
}
