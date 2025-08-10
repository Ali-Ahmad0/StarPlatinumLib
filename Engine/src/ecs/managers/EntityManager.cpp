#include "EntityManager.hpp"

void EntityManager::Init() 
{
    for (EntityID e = 0; e < MAX_ENTITIES; e++)
    {
        m_availableEntities.push(e);
    }
}

bool EntityManager::IsActive(const EntityID entity)
{
    return entity < MAX_ENTITIES && m_entityStatus[entity];
}

EntityID EntityManager::CreateEntity() 
{
    if (m_entityCount < MAX_ENTITIES)
    {
        const EntityID entity = m_availableEntities.front();
        m_availableEntities.pop();
        m_entityStatus[entity] = true;

        m_entityCount++;
        return entity;
    }

    throw std::runtime_error("[RUNTIME ERROR]: Max entity limit reached");
    
}

void EntityManager::DeleteEntity(EntityID entity)
{
    if (IsActive(entity))
    {
        m_availableEntities.push(entity);
        m_entityStatus[entity] = false;
        m_entityCount--;
    }
    else 
    {
        fprintf(stderr, "[ERROR]: Cannot delete non existent entity\n");
    }
    
}

size_t EntityManager::GetEntityCount()
{
    return m_entityCount;
}

void EntityManager::SetSignature(EntityID entity, Signature signature)
{
    if (IsActive(entity)) 
    {
        m_signatures[entity] = signature;
    }
    else 
    {
        fprintf(stderr, "[ERROR]: Cannot set signature on non existent entity\n");
    }
}

Signature EntityManager::GetSignature(EntityID entity)
{
    if (IsActive(entity)) 
    {
        return m_signatures[entity];
    }

    throw std::runtime_error("[RUNTIME ERROR]: Cannot get signature on non existent entity");
}
