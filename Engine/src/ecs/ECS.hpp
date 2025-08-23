#pragma once

#include "Definitions.hpp"
#include "components/Components.hpp"
#include "components/SparseSet.hpp"

#include "managers/EntityManager.hpp"
#include "managers/SystemManager.hpp"

// Entity Component System Manager
class ECS
{

public:
    // Initialize ECS
    static void Init();

    static EntityID CreateEntity();
    static void DeleteEntity(EntityID entity);
    static size_t GetEntityCount();

    template <typename T>
    static void RegisterComponent()
    {
        const std::type_index typeIndex(typeid(T));
        
        auto sparseSet = std::make_unique<SparseSet<T>>();        
        sparseSet->Init();

        s_sparseSets[typeIndex] = std::move(sparseSet);

        // Assign component ID
        s_componentRegistry[typeIndex] = s_nextComponentId;
        s_nextComponentId++;

        printf("[INFO]: Registered component: %s\n", typeIndex.name());
    }

    template <typename T>
    static void AddComponent(EntityID entity, T component)
    {
        if (!s_entityManager->IsActive(entity)) 
        {
            fprintf(stderr, "[ERROR]: Cannot add component on non existing entity\n");
            return;
        }

        getComponentSparseSet<T>()->AddData(entity, component);
        updateEntitySignature<T>(entity, true);
    }

    template <typename T>
    static void RemoveComponent(EntityID entity)
    {
        if (!s_entityManager->IsActive(entity))
        {
            fprintf(stderr, "[ERROR]: Cannot remove component on non existing entity\n");
            return;
        }

        getComponentSparseSet<T>()->RemoveData(entity);
        updateEntitySignature<T>(entity, false);
    }

    template <typename T>
    static bool HasComponent(EntityID entity) 
    {
        return getComponentSparseSet<T>()->HasData(entity);
    }

    template <typename T>
    static T* GetComponent(EntityID entity)
    {
        return getComponentSparseSet<T>()->GetData(entity);
    }

    template <typename T>
    static ComponentID GetComponentID()
    {
        std::type_index typeIndex = typeid(T);
        
        // Find component ID of component
        if (s_componentRegistry.find(typeIndex) != s_componentRegistry.end())
        {
            // Return ID
            return s_componentRegistry[typeIndex];
        }
        throw std::runtime_error("[RUNTIME ERROR]: Cannot get ID on unregistered component");
    }


    template <typename T>
    static std::shared_ptr<T> RegisterSystem() 
    {
        return s_systemManager->RegisterSystem<T>();
    }

    template <typename T>
    static std::shared_ptr<T> GetSystem()
    {
        return std::static_pointer_cast<T>(s_systemManager->GetSystem<T>());
    }

    template <typename T, typename U>
    static void AddComponentToSystem() 
    {
        Signature signature = s_systemManager->GetSignature<U>();
        signature.set(GetComponentID<T>(), true);
        s_systemManager->SetSignature<U>(signature);
    }

private:
    // Managers
    static std::unique_ptr<EntityManager> s_entityManager;
    static std::unique_ptr<SystemManager> s_systemManager;

    // Map that assigns an ID to each component type
    static std::unordered_map<std::type_index, ComponentID> s_componentRegistry;

    // Map that stores all registered component pools
    static std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>> s_sparseSets;

    // Next component ID
    static ComponentID s_nextComponentId;

    // Get the component pool for a specific type
    template <typename T>
    static SparseSet<T>* getComponentSparseSet() 
    {
        const std::type_index typeIndex(typeid(T));
        const auto it = s_sparseSets.find(typeIndex);
        if (it != s_sparseSets.end())
        {
            return static_cast<SparseSet<T>*>(it->second.get());
        }
        throw std::runtime_error("[RUNTIME ERROR]: Component map for type not registered");
    }

    // Update the signature of an entity and notify system manager
    template <typename T>
    static void updateEntitySignature(EntityID entity, bool value) 
    {
        Signature signature = s_entityManager->GetSignature(entity);
        signature.set(GetComponentID<T>(), value);
        s_entityManager->SetSignature(entity, signature);

        s_systemManager->OnEntitySignatureChanged(entity, signature);
    }
};

