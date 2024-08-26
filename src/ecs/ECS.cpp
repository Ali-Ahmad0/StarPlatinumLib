//#include "ECS.hpp"
//
//// Create a new entity
//EntityID ECS::createEntity() 
//{
//    // Add transform component by default
//    addComponent(nextEntity, std::make_shared<TransformComponent>(Vector2(0, 0), 1));
//
//    activeEntityList.push_back(nextEntity);
//    return nextEntity++;
//}
//
//void ECS::deleteEntity(EntityID entity)
//{
//    // Find entity
//    auto entityIt = entities.find(entity);
//    if (entityIt != entities.end())
//    {
//        // Remove all components of the entity
//        entityIt->second.clear();
//
//        // Remove the entity itself
//        entities.erase(entity);
//
//        // Update activeEntityList
//        auto& activeEntities = activeEntityList;  // Reference for convenience
//
//        // Ensure the entity exists in the list
//        auto it = std::find(activeEntities.begin(), activeEntities.end(), entity);
//        if (it != activeEntities.end())
//        {
//            // Swap with the last element
//            if (activeEntities.size() > 1)
//            {
//                // Get the last element
//                EntityID lastEntity = activeEntities.back();
//
//                // Swap the current entity with the last one
//                *it = lastEntity;
//            }
//
//            // Remove the last element
//            activeEntities.pop_back();
//        }
//        else
//        {
//            fprintf(stderr, "Entity %zu not found in activeEntityList\n", entity);
//        }
//    }
//    else
//    {
//        fprintf(stderr, "Entity %zu not found\n", entity);
//    }
//}
//
//// Get the next available entity ID
//EntityID ECS::getNextEntity() const 
//{
//    return nextEntity;
//}
//
//// Get total number of entities
//EntityID ECS::getEntityCount() const
//{
//    return activeEntityList.size();
//}
//
//std::unordered_map<EntityID, std::unordered_map<std::type_index, std::shared_ptr<Component>>> ECS::getAllEntities() const 
//{
//    return entities;
//}
//
//// Add a component
//void ECS::addComponent(EntityID entity, std::shared_ptr<Component> component) 
//{
//    entities[entity][std::type_index(typeid(*component))] = component;
//}
