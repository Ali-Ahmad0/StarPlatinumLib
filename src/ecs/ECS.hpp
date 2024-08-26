#ifndef ECS_HPP
#define ECS_HPP

#include "defintions.hpp"
#include "components/Components.hpp"
#include "components/ComponentPool.hpp"

// Entity Component System Manager
class ECS
{

public:
    // Entity related methods

    EntityID createEntity()
    {
        EntityID id = nextEntity++;
        activeEntityList.push_back(id);
        return id;

        printf("Entity created %zu", id);
    }

    void deleteEntity(EntityID entity)
    {
        // Remove entity from active entity list
        auto it = std::find(activeEntityList.begin(), activeEntityList.end(), entity);
        if (it != activeEntityList.end())
        {
            activeEntityList.erase(it);
        }

        // Remove components associated with the entity
        for (auto& pair : componentPools)
        {
            pair.second->EntityDestroyed(entity);
        }
    }

    std::vector<EntityID> getAllEntities()
    {
        return activeEntityList;
    }

    // Register a component type by creating its pool
    template <typename T>
    void registerComponent()
    {
        std::type_index typeIndex(typeid(T));
        componentPools[typeIndex] = std::make_unique<ComponentPool<T>>();
    }

    // Add a component to an entity
    template <typename T>
    void addComponent(EntityID entity, T component)
    {
        getComponentPool<T>()->AddData(entity, component);
    }

    // Remove a component from an entity
    template <typename T>
    void removeComponent(EntityID entity)
    {
        getComponentPool<T>()->RemoveData(entity);
    }

    // Check if an entity has a specific component
    template <typename T>
    bool hasComponent(EntityID entity) const
    {
        return getComponentPool<T>()->HasData(entity);
    }

    // Get a reference to an entity's component
    template <typename T>
    T* getComponent(EntityID entity)
    {
        return getComponentPool<T>()->GetData(entity);
    }

private:
    // next available entity ID
    EntityID nextEntity = 0;

    // Map that stores all registered component pools
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> componentPools;

    // Active entity list to keep track of all active entities
    std::vector<EntityID> activeEntityList;

    // Helper function to get the component pool for a specific type
    template <typename T>
    ComponentPool<T>* getComponentPool() const
    {
        std::type_index typeIndex(typeid(T));
        auto it = componentPools.find(typeIndex);
        if (it != componentPools.end())
        {
            return static_cast<ComponentPool<T>*>(it->second.get());
        }
        throw std::runtime_error("Component pool for type not registered.");
    }
};

#endif // ECS_HPP
