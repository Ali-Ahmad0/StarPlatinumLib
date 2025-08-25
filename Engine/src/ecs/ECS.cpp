#include "ECS.hpp"

std::unique_ptr<EntityManager> ECS::s_entityManager = nullptr;
std::unique_ptr<SystemManager> ECS::s_systemManager = nullptr;

std::unordered_map<std::type_index, ComponentID> ECS::s_componentRegistry{};
std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>> ECS::s_sparseSets{};

ComponentID ECS::s_nextComponentId = 0;


void ECS::Init() 
{
    s_entityManager = std::make_unique<EntityManager>();
    s_systemManager = std::make_unique<SystemManager>();

    s_entityManager->Init();
}

EntityID ECS::CreateEntity()
{
    return s_entityManager->CreateEntity();
}

void ECS::DeleteEntity(EntityID entity)
{
    // Handle entity record and system updates
    s_entityManager->DeleteEntity(entity);
    s_systemManager->OnEntityDestroyed(entity);
    
    // Remove components associated with the entity
    for (auto& pair : s_sparseSets)
    {
        pair.second->OnEntityDestroyed(entity);
    }
}

void ECS::DeleteAllEntities()
{
    // Handle clearing entity and system data
    s_entityManager->ClearEntites();
    s_systemManager->OnAllEntitesDestroyed();

    // Clear all component data from sparse sets
    for (auto& pair : s_sparseSets) 
    {
        pair.second->OnAllEntitiesDestroyed();
    }
}

size_t ECS::GetEntityCount() 
{
    return s_entityManager->GetEntityCount();
}