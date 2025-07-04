#pragma once

#include "definitions.hpp"
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

    // Entity related methods
    static EntityID CreateEntity();
    static void DeleteEntity(EntityID entity);
    static size_t GetEntityCount();

    // Component related methods
    template <typename T>
    static void RegisterComponent()
    {
        const std::type_index typeIndex(typeid(T));
        
        auto sparseSet = std::make_unique<SparseSet<T>>();        
        sparseSet->Init();

        sparseSets[typeIndex] = std::move(sparseSet);

        // Assign component ID
        componentRegistry[typeIndex] = nextComponent;
        nextComponent++;

        printf("[INFO]: Registered component: %s\n", typeIndex.name());
    }

    template <typename T>
    static void AddComponent(EntityID entity, T component)
    {
        if (!entityManager->IsActive(entity)) 
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
        if (!entityManager->IsActive(entity))
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
        if (componentRegistry.find(typeIndex) != componentRegistry.end())
        {
            // Return ID
            return componentRegistry[typeIndex];
        }
        throw std::runtime_error("[RUNTIME ERROR]: Cannot get ID on unregistered component");
    }

    // System related methods
    template <typename T>
    static std::shared_ptr<T> RegisterSystem() 
    {
        return systemManager->RegisterSystem<T>();
    }

    template <typename T>
    static std::shared_ptr<T> GetSystem()
    {
        return std::static_pointer_cast<T>(systemManager->GetSystem<T>());
    }

    template <typename T, typename U>
    static void AddComponentToSystem() 
    {
        Signature signature = systemManager->GetSignature<U>();
        signature.set(GetComponentID<T>(), true);
        systemManager->SetSignature<U>(signature);
        
    }
   
private:
    // Managers
    static std::unique_ptr<EntityManager> entityManager;
    static std::unique_ptr<SystemManager> systemManager;

    // Map that assigns an ID to each component type
    static std::unordered_map<std::type_index, ComponentID> componentRegistry;

    // Map that stores all registered component pools
    static std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>> sparseSets;

    // Next component ID
    static ComponentID nextComponent;

    // Get the component pool for a specific type
    template <typename T>
    static SparseSet<T>* getComponentSparseSet() 
    {
        const std::type_index typeIndex(typeid(T));
        const auto it = sparseSets.find(typeIndex);
        if (it != sparseSets.end())
        {
            return static_cast<SparseSet<T>*>(it->second.get());
        }
        throw std::runtime_error("[RUNTIME ERROR]: Component map for type not registered");
    }

    // Update the signature of an entity and notify system manager
    template <typename T>
    static void updateEntitySignature(EntityID entity, bool value) 
    {
        Signature signature = entityManager->GetSignature(entity);
        signature.set(GetComponentID<T>(), value);
        entityManager->SetSignature(entity, signature);

        systemManager->OnEntitySignatureChanged(entity, signature);
    }
};

