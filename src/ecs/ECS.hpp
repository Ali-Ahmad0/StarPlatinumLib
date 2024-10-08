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
        for (auto& pair : sparseSets)
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
        
        auto sparseSet = std::make_unique<ComponentSparseSet<T>>();        
        sparseSet->Init();

        sparseSets[typeIndex] = std::move(sparseSet);

        // Assign component ID
        componentRegistry[typeIndex] = nextComponent;
        nextComponent++;
    }

    template <typename T>
    void AddComponent(EntityID entity, T component)
    {
        if (!entityManager->IsActive(entity)) 
        {
            fprintf(stderr, "Cannot add component on non existing entity");
            return;
        }

        getComponentSparseSet<T>()->AddData(entity, component);
        updateEntitySignature<T>(entity, true);
    }

    template <typename T>
    void RemoveComponent(EntityID entity)
    {
        if (!entityManager->IsActive(entity))
        {
            fprintf(stderr, "Cannot remove component on non existing entity");
            return;
        }

        getComponentSparseSet<T>()->RemoveData(entity);
        updateEntitySignature<T>(entity, false);
    }

    template <typename T>
    bool HasComponent(EntityID entity) const
    {
        return getComponentSparseSet<T>()->HasData(entity);
    }

    template <typename T>
    T* GetComponent(EntityID entity)
    {
        return getComponentSparseSet<T>()->GetData(entity);
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
    std::unordered_map<std::type_index, std::unique_ptr<IComponentSparseSet>> sparseSets{};

    // Next component ID
    ComponentID nextComponent = 0;

    // Get the component pool for a specific type
    template <typename T>
    ComponentSparseSet<T>* getComponentSparseSet() const
    {
        const std::type_index typeIndex(typeid(T));
        const auto it = sparseSets.find(typeIndex);
        if (it != sparseSets.end())
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

