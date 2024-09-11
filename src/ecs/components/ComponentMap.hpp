#ifndef COMPONENT_POOL_HPP
#define COMPONENT_POOL_HPP

#include "../defintions.hpp"

// Interface for a component map
class IComponentMap
{
public:
    virtual ~IComponentMap() = default;
    virtual void OnEntityDestroyed(EntityID e) = 0;
};

template <typename T>
class ComponentMap : public IComponentMap
{
public:
    // Add a component of Type T for entity e
    void AddData(EntityID e, T component)
    {
        registries[e] = std::make_unique<T>(component);
    }

    // Check if entity e has a component of type T
    bool HasData(EntityID e)
    {
        return registries.find(e) != registries.end();
    }

    // Get a pointer to the component of Type T for entity e
    T* GetData(EntityID e)
    {
        auto it = registries.find(e);
        if (it != registries.end())
        {
            return it->second.get();
        }
        fprintf(stderr, "Entity %zu does not have component of type: %s", e, typeid(T).name());
        return nullptr;
    }

    void RemoveData(EntityID e)
    {
        auto it = registries.find(e);
        if (it != registries.end())
        {
            registries.erase(it);
        }
        else
        {
            fprintf(stderr, "Entity %zu does not have component of type: %s", e, typeid(T).name());
        }
    }

    // Remove data for entity when it is destroyed
    void OnEntityDestroyed(EntityID e) override
    {
        RemoveData(e);
    }

private:
    // Entity and it's corresponding component of type T
    std::unordered_map<EntityID, std::unique_ptr<T>> registries;
};

#endif // !COMPONENT_POOL_HPP
