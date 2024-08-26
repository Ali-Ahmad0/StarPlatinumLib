#ifndef ECS_HPP
#define ECS_HPP

#include "defintions.hpp"
#include "components/Components.hpp"
#include "components/ComponentPool.hpp"

// Entity Component System Manager
class ECS
{

public:
    void Init();

    // Entity related methods
    EntityID CreateEntity();
    void DeleteEntity(EntityID entity);

    std::vector<EntityID> GetAllEntities()
    {
        return activeEntityList;
    }

    size_t GetEntityCount();

    // Register a component type by creating its pool
    template <typename T>
    void RegisterComponent()
    {
        std::type_index typeIndex(typeid(T));
        componentPools[typeIndex] = std::make_unique<ComponentPool<T>>();
    }

    // Add a component to an entity
    template <typename T>
    void AddComponent(EntityID entity, T component)
    {
        getComponentPool<T>()->AddData(entity, component);
    }

    // Remove a component from an entity
    template <typename T>
    void RemoveComponent(EntityID entity)
    {
        getComponentPool<T>()->RemoveData(entity);
    }

    // Check if an entity has a specific component
    template <typename T>
    bool HasComponent(EntityID entity) const
    {
        return getComponentPool<T>()->HasData(entity);
    }

    // Get a reference to an entity's component
    template <typename T>
    T* GetComponent(EntityID entity)
    {
        return getComponentPool<T>()->GetData(entity);
    }

private:
    // Queue of available entity IDs
    std::queue<EntityID> availableEntities;

    // Total entities
    size_t entityCount = 0;

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
