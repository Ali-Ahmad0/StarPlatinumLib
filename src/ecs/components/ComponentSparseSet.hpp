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
    // Reserve memory at initialization to reduce allocations
    void Init()
    {
        dense.reserve(MAX_COMPONENTS);
        denseToEntity.reserve(MAX_COMPONENTS);
    }

    // Add a component of Type T for entity e
    void AddData(EntityID e, T component)
    {
        // Overrite existing data
        if (HasData(e))
        {
            *GetData(e) = std::move(component);
            return;
        }

        // Add component to the dense array
        SetDenseIndex(e, dense.size());
        dense.push_back(std::move(component));
        denseToEntity.push_back(e);
        
    }

    // Check if entity e has a component of type T
    bool HasData(EntityID e)
    {
        return GetDenseIndex(e) != NULL_INDEX;
    }

    // Get a pointer to the component of Type T for entity e
    T* GetData(EntityID e)
    {
        size_t index = GetDenseIndex(e);
        if (index == NULL_INDEX)
        {
            fprintf(stderr, "Entity %u does not have component of type: %s\n", e, typeid(T).name());
            return nullptr;
        }
        
        return &dense[index];
    }

    // Remove a component of Type T for entity e
    void RemoveData(EntityID e)
    {
        size_t index = GetDenseIndex(e);
        if (index == NULL_INDEX)
        {
            fprintf(stderr, "Entity %u does not have component of type: %s\n", e, typeid(T).name());
            return;
        }

        // Entity of the last dense element
        EntityID lastEntity = denseToEntity.back();

        // Swap the element to be deleted with the last element
        std::swap(dense[index], dense.back());
        std::swap(denseToEntity[index], denseToEntity.back());

        // Update the index for entities
        SetDenseIndex(lastEntity, index);
        SetDenseIndex(e, NULL_INDEX);

        // Remove the last component and its corresponding entity ID
        dense.pop_back();
        denseToEntity.pop_back();
    }

    // Remove data for entity when it is destroyed
    void OnEntityDestroyed(EntityID e) override
    {
        RemoveData(e);
    }

private:
    std::vector<std::vector<size_t>> sparsePages; 
    std::vector<T> dense; 
    
    std::vector<EntityID> denseToEntity; 
    
    static constexpr size_t NULL_INDEX = std::numeric_limits<size_t>::max();
    static constexpr size_t MAX_PAGE_SIZE = 512;

    // Map an index of a component to an entity 
    void SetDenseIndex(EntityID e, size_t index)
    {
        size_t page = e / MAX_PAGE_SIZE;
        size_t pageIndex = e % MAX_PAGE_SIZE;

        // Create a new page if needed
        if (page >= sparsePages.size())
        {

            sparsePages.resize(page + 1);
            sparsePages[page].resize(MAX_PAGE_SIZE, NULL_INDEX);
        }

        // Set the dense index for this entity
        sparsePages[page][pageIndex] = index;
    }

    // Get the dense index for an entity
    size_t GetDenseIndex(EntityID e)
    {
        size_t page = e / MAX_PAGE_SIZE;
        size_t pageIndex = e % MAX_PAGE_SIZE;

        // Check if the page exists and the entity has a valid index
        if (page < sparsePages.size())
        {
            return sparsePages[page][pageIndex];
        }

        // Return NULL_INDEX if the entity doesn't have a component
        return NULL_INDEX;
    }
};