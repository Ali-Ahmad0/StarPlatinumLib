#pragma once

#include "defintions.hpp"
#include "components/Components.hpp"
#include "components/ComponentSparseSet.hpp"

#include "managers/EntityManager.hpp"
#include "managers/SystemManager.hpp"

// Entity Component System Manager
class ECS
{

public:
    // Initialize ECS
    void Init() 
    {
        entityManager = std::make_unique<EntityManager>();
        systemManager = std::make_unique<SystemManager>();
        entityManager->Init();
    }

    // Entity related methods
    EntityID CreateEntity() const
    {
        return entityManager->CreateEntity();
    }

    void DeleteEntity(const EntityID entity) const
    {
        entityManager->DeleteEntity(entity);

        // Remove components associated with the entity
        for (auto& pair : componentPools)
        {
            pair.second->OnEntityDestroyed(entity);
        }

        systemManager->OnEntityDestroyed(entity);
    }

    size_t GetEntityCount() const
    {
        return entityManager->GetEntityCount();
    }

    // Component related methods
    template <typename T>
    void RegisterComponent()
    {
        const std::type_index typeIndex(typeid(T));
        componentPools[typeIndex] = std::make_unique<ComponentSparseSet<T>>();

        // Assign component ID
        componentRegistry[typeIndex] = nextComponent;
        nextComponent++;
    }

    template <typename T>
    void AddComponent(EntityID entity, T component)
    {
        getComponentMap<T>()->AddData(entity, component);
        updateEntitySignature<T>(entity, true);
    }

    template <typename T>
    void RemoveComponent(EntityID entity)
    {
        getComponentMap<T>()->RemoveData(entity);
        updateEntitySignature<T>(entity, false);
    }

    template <typename T>
    bool HasComponent(EntityID entity) const
    {
        return getComponentMap<T>()->HasData(entity);
    }

    template <typename T>
    T* GetComponent(EntityID entity)
    {
        // Returns reference to a component
        return getComponentMap<T>()->GetData(entity);
    }

    template <typename T>
    ComponentID GetComponentID()
    {
        std::type_index typeIndex = typeid(T);
        
        // Find component ID of component
        if (componentRegistry.find(typeIndex) != componentRegistry.end())
        {
            // Return ID
            return componentRegistry[typeIndex];
        }
        throw std::runtime_error("Cannot get ID on unregistered component");
    }

    // System related methods
    template <typename T>
    std::shared_ptr<T> RegisterSystem() 
    {
        return systemManager->RegisterSystem<T>();
    }

    template <typename T>
    std::shared_ptr<T> GetSystem()
    {
        return std::static_pointer_cast<T>(systemManager->GetSystem<T>());
    }

    template <typename T, typename U>
    void AddComponentToSystem() 
    {
        Signature signature = systemManager->GetSignature<U>();
        signature.set(GetComponentID<T>(), true);
        systemManager->SetSignature<U>(signature);
        
    }
   
private:
    // Managers
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<SystemManager> systemManager;

    // Map that assigns an ID to each component type
    std::unordered_map<std::type_index, ComponentID> componentRegistry{};

    // Map that stores all registered component pools
    std::unordered_map<std::type_index, std::unique_ptr<IComponentSparseSet>> componentPools{};

    // Next component ID
    ComponentID nextComponent = 0;

    // Get the component pool for a specific type
    template <typename T>
    ComponentSparseSet<T>* getComponentMap() const
    {
        const std::type_index typeIndex(typeid(T));
        const auto it = componentPools.find(typeIndex);
        if (it != componentPools.end())
        {
            return static_cast<ComponentSparseSet<T>*>(it->second.get());
        }
        throw std::runtime_error("Component map for type not registered.");
    }

    // Update the signature of an entity and notify system manager
    template <typename T>
    void updateEntitySignature(EntityID entity, bool value) 
    {
        Signature signature = entityManager->GetSignature(entity);
        signature.set(GetComponentID<T>(), value);
        entityManager->SetSignature(entity, signature);

        systemManager->OnEntitySignatureChanged(entity, signature);
    }
};

