#pragma once

#include "../defintions.hpp"

// Interface for a component map
class IComponentSparseSet
{
public:
    virtual ~IComponentSparseSet() = default;
    virtual void OnEntityDestroyed(EntityID e) = 0;
};

template <typename T>
class ComponentSparseSet final : public IComponentSparseSet
{
public:
    // Reserve memory for sparse and dense arrays
    void Init() 
    {
        sparse.reserve(MAX_ENTITIES);
        dense.reserve(MAX_COMPONENTS);
    }

    // Add a component of Type T for entity e
    void AddData(EntityID e, T component)
    {
        // Ensure the sparse vector has enough entries
        if (e >= sparse.size())
        {
            sparse.resize(e + 1, NULL_INDEX);
        }

        // Set the dense index in the sparse vector
        sparse[e] = dense.size(); 

        // Add component to the dense array
        dense.push_back(std::move(component));    
    }

    // Check if entity e has a component of type T
    bool HasData(EntityID e)
    {
        if (e >= sparse.size())
        {
            return false;
        }
        return sparse[e] != NULL_INDEX;
    }

    // Get a pointer to the component of Type T for entity e
    T* GetData(EntityID e)
    {
        if (!HasData(e))
        {
            fprintf(stderr, "Entity %zu does not have component of type: %s\n", e, typeid(T).name());
            return nullptr;
        }

        return &dense[sparse[e]];
    }

    // Remove a component of Type T for entity e
    void RemoveData(EntityID e)
    {
        if (!HasData(e))
        {
            fprintf(stderr, "Entity %zu does not have component of type: %s\n", e, typeid(T).name());
            return;
        }
        
        // Index of component to delete
        size_t componentIndex = sparse[e];

        // Entity of the last dense element
        EntityID backEntity = sparse.back();  
        
        // Swap with the last element in dense
        std::swap(dense[componentIndex], dense.back());

        // Mark the sparse entry as removed
        sparse[e] = NULL_INDEX;

        if (backEntity != NULL_INDEX)
        {
            sparse[backEntity] = componentIndex;
        }
        
        // Remove the last component
        dense.pop_back();  
    }

    // Remove data for entity when it is destroyed
    void OnEntityDestroyed(EntityID e) override
    {
        RemoveData(e);
    }

private:
    // Sparse set implementation
    std::vector<size_t> sparse;
    std::vector<T> dense;

    static constexpr size_t NULL_INDEX = std::numeric_limits<size_t>::max();

};