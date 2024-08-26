#ifndef ECS_HPP
#define ECS_HPP

#include "defintions.hpp"
#include "components/Components.hpp"
#include "components/ComponentPool.hpp"

#include "EntityManager.hpp"

// Entity Component System Manager
class ECS
{

public:
    void Init() 
    {
        entityManager = std::make_unique<EntityManager>();
        entityManager->Init();
    }

    // Entity related methods
    EntityID CreateEntity() 
    {
        return entityManager->CreateEntity();
    }

    void DeleteEntity(EntityID entity) 
    {
        entityManager->DeleteEntity(entity);
    }

    std::vector<EntityID> GetAllEntities() 
    {
        return entityManager->GetAllEntities();
    }

    size_t GetEntityCount() 
    {
        return entityManager->GetEntityCount();
    }

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
    std::unique_ptr<EntityManager> entityManager;

    // Map that stores all registered component pools
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> componentPools;

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
