#include "ECS.hpp"

// Create a new entity
Entity EntityManager::createEntity() 
{
    // Add transform component by default
    addComponent(nextEntity, std::make_shared<TransformComponent>(Vector2(0, 0), 1));
    return nextEntity++;
}

// Delete an entity and all its components
void EntityManager::deleteEntity(Entity entity)
{
    // Find entity
    auto entityIt = entities.find(entity);
    if (entityIt != entities.end())
    {
        // Remove all components of the entity
        entityIt->second.clear();

        // Remove the entity itself
        entities.erase(entity);
    }
    else
    {
        fprintf(stderr, "Entity %zu not found\n", entity);
    }
}

// Get the next available entity ID
Entity EntityManager::getNextEntity() const 
{
    return nextEntity;
}

// Get total number of entities
Entity EntityManager::getEntityCount() const
{
    return entities.size();
}

std::unordered_map<Entity, std::unordered_map<std::type_index, std::shared_ptr<Component>>> EntityManager::getAllEntities() const 
{
    return entities;
}

// Add a component
void EntityManager::addComponent(Entity entity, std::shared_ptr<Component> component) 
{
    entities[entity][std::type_index(typeid(*component))] = component;
}
