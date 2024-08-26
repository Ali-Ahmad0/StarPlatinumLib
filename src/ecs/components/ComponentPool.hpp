#ifndef COMPONENT_POOL_HPP
#define COMPONENT_POOL_HPP

#include "../defintions.hpp"

class IComponentPool
{
public:
    virtual ~IComponentPool() = default;
    virtual void EntityDestroyed(EntityID e) = 0;
};

template <typename T>
class ComponentPool : public IComponentPool
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
        auto registry = registries.find(e);
        if (HasData(e))
        {
            return registry->second.get();
        }

        else
        {
            fprintf(stderr, "Entity %zu does not have component of type: %s", e, typeid(T).name());
        }
        return nullptr;
    }

    // Remove component of type T for entity e
    void RemoveData(EntityID e)
    {
        auto registry = registries.find(e);
        if (HasData(e))
        {
            registries.erase(registry);
        }

        else
        {
            fprintf(stderr, "Entity %zu does not have component of type: %s", e, typeid(T).name());
        }
    }

    // Remove data for entity when it is destroyed
    void EntityDestroyed(EntityID e) override
    {
        RemoveData(e);
    }

private:
    // Entity and it's corresponding component of type T
    std::unordered_map<EntityID, std::unique_ptr<T>> registries;
};

#endif // !COMPONENT_POOL_HPP
