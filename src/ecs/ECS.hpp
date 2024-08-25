#ifndef ECS_HPP
#define ECS_HPP

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <cstdio>  

#include "../main/Game.hpp"
#include "Components.hpp"


// Entity type
using EntityID = std::size_t;

// Entity Components registry
class ECS
{
private:
    // Count of total living entities
    size_t entityCount = 0;

    std::unordered_map<EntityID, std::unordered_map<std::type_index, std::shared_ptr<Component>>> entities;
    EntityID nextEntity = 0; // next available entity index

public:
    std::vector<EntityID> activeEntityList;

    EntityID createEntity();
    void deleteEntity(EntityID entity);

    EntityID getNextEntity() const;
    EntityID getEntityCount() const;
    std::unordered_map<EntityID, std::unordered_map<std::type_index, std::shared_ptr<Component>>> getAllEntities() const;

    void addComponent(EntityID entity, std::shared_ptr<Component> component);

    // Remove a specific type of component
    template <typename T>
    void removeComponent(EntityID entity)
    {
        // Find an entity
        auto entityIt = entities.find(entity);
        if (entityIt != entities.end())
        {
            // Get unordered map of components
            auto& components = entityIt->second;

            // Find the component
            auto compIt = components.find(std::type_index(typeid(T)));
            if (compIt != components.end())
            {
                // Remove component
                components.erase(compIt);
            }
            else
            {
                fprintf(stderr, "Component of type %s not found for entity %zu\n",
                    typeid(T).name(), entity);
            }
        }
        else
        {
            fprintf(stderr, "Entity %zu not found\n", entity);
        }
    }

    // Checks if that component exists or not
    template <typename T>
    bool hasComponent(EntityID entity) const
    {
        // Find the entity
        auto entityIt = entities.find(entity);
        if (entityIt != entities.end())
        {
            // Find it's component
            auto compIt = entityIt->second.find(std::type_index(typeid(T)));

            // Check if component exists        
            return compIt != entityIt->second.end();
        }
        return false;
    }

    // Returns reference to a component
    template <typename T>
    T* getComponent(EntityID entity)
    {
        // Find the entity
        auto entityIt = entities.find(entity);
        if (entityIt != entities.end())
        {
            // Find its component 
            auto compIt = entityIt->second.find(std::type_index(typeid(T)));
            if (compIt != entityIt->second.end())
            {
                return static_cast<T*>(compIt->second.get());
            }
            else
            {
               fprintf(stderr, "Component of type %s not found for entity %zu\n",
                    typeid(T).name(), entity);
            }
        }
        else
        {
            fprintf(stderr, "Entity %zu not found\n", entity);
        }
        return nullptr;
    }
};

#endif // ECS_HPP
