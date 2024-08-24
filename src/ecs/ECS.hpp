#ifndef ECS_HPP
#define ECS_HPP

#include <cstddef>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cstdio>  

#include "../main/Game.hpp"
#include "Components.hpp"

// Entity type
using Entity = std::size_t;

// Entity Components registry
class EntityManager
{
private:
    std::unordered_map<Entity, std::unordered_map<std::type_index, std::shared_ptr<Component>>> entities;
    Entity nextEntity = 0; // next available entity index

public:

    Entity createEntity();
    void deleteEntity(Entity entity);

    Entity getNextEntity() const;
    Entity getEntityCount() const;
    std::unordered_map<Entity, std::unordered_map<std::type_index, std::shared_ptr<Component>>> getAllEntities() const;

    void addComponent(Entity entity, std::shared_ptr<Component> component);

    // Remove a specific type of component
    template <typename T>
    void removeComponent(Entity entity)
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
    bool hasComponent(Entity entity) const
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
    T* getComponent(Entity entity)
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
