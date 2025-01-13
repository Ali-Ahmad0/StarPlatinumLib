#include "ECS.hpp"

std::unique_ptr<EntityManager> ECS::entityManager = nullptr;
std::unique_ptr<SystemManager> ECS::systemManager = nullptr;

std::unordered_map<std::type_index, ComponentID> ECS::componentRegistry{};
std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>> ECS::sparseSets{};

ComponentID ECS::nextComponent = 0;


void ECS::Init() 
{
    entityManager = std::make_unique<EntityManager>();
    systemManager = std::make_unique<SystemManager>();

    entityManager->Init();
}

EntityID ECS::CreateEntity()
{
    return entityManager->CreateEntity();
}

void ECS::DeleteEntity(EntityID entity) 
{
    entityManager->DeleteEntity(entity);

    // Remove components associated with the entity
    for (auto& pair : sparseSets)
    {
        pair.second->OnEntityDestroyed(entity);
    }

    systemManager->OnEntityDestroyed(entity);
}

size_t ECS::GetEntityCount() 
{
    return entityManager->GetEntityCount();
}