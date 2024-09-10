#ifndef ECS_HPP
#define ECS_HPP

#include "defintions.hpp"
#include "components/Components.hpp"
#include "components/ComponentMap.hpp"

#include "managers/EntityManager.hpp"
#include "managers/SystemManager.hpp"

// Entity Component System Manager
class ECS
{

public:
    void Init() 
    {
        entityManager = std::make_unique<EntityManager>();
        systemManager = std::make_unique<SystemManager>();
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

        // Remove components associated with the entity
        for (auto& pair : componentPools)
        {
            pair.second->EntityDestroyed(entity);
        }

        systemManager->OnEntityDestroyed(entity);
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
        componentPools[typeIndex] = std::make_unique<ComponentMap<T>>();

        // Assign component ID
        componentRegistry[typeIndex] = nextComponent;
        nextComponent++;
    }

    // Add a component to an entity
    template <typename T>
    void AddComponent(EntityID entity, T component)
    {
        getComponentPool<T>()->AddData(entity, component);
        updateEntitySignature<T>(entity, true);
    }

    // Remove a component from an entity
    template <typename T>
    void RemoveComponent(EntityID entity)
    {
        getComponentPool<T>()->RemoveData(entity);
        updateEntitySignature<T>(entity, false);
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

    // Get the component ID for a specific type
    template <typename T>
    ComponentID GetComponentID()
    {
        std::type_index typeIndex = typeid(T);

        if (componentRegistry.find(typeIndex) != componentRegistry.end())
        {
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

    template <typename T>
    void SetSystemSignature(Signature signature) 
    {
        return systemManager->SetSignatrue<T>(signature);
    }
   
private:
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<SystemManager> systemManager;

    // Map that assigns an ID to each component type
    std::unordered_map<std::type_index, ComponentID> componentRegistry{};

    // Map that stores all registered component pools
    std::unordered_map<std::type_index, std::unique_ptr<IComponentMap>> componentPools{};

    // Next component ID
    ComponentID nextComponent = 0;

    // Get the component pool for a specific type
    template <typename T>
    ComponentMap<T>* getComponentPool() const
    {
        std::type_index typeIndex(typeid(T));
        auto it = componentPools.find(typeIndex);
        if (it != componentPools.end())
        {
            return static_cast<ComponentMap<T>*>(it->second.get());
        }
        throw std::runtime_error("Component pool for type not registered.");
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

#endif // ECS_HPP
