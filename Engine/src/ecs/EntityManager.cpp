#include "EntityManager.hpp"
#include <numeric>

void EntityManager::Init() 
{
    // Initialize available IDs
    std::deque<EntityID> temp;
    temp.resize(MAX_ENTITIES);
    std::iota(temp.begin(), temp.end(), 0);
    m_availableEntities = std::queue<EntityID>(temp);
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
        m_signatures[entity].reset();
    }
    else 
    {
        fprintf(stderr, "[ERROR]: Cannot delete non existent entity\n");
    }
    
}

void EntityManager::ClearEntites()
{
    // Reset entity status
    std::fill(m_entityStatus.begin(), m_entityStatus.end(), false);

    // Reset available IDs
    std::deque<EntityID> temp;
    temp.resize(MAX_ENTITIES);
    std::iota(temp.begin(), temp.end(), 0);
    m_availableEntities = std::queue<EntityID>(temp);

    // Reset the signatures
    for (auto& signature: m_signatures) 
    { 
        signature.reset(); 
    }

    // Reset the entity count
    m_entityCount = 0;
}

uint16_t EntityManager::GetEntityCount()
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
