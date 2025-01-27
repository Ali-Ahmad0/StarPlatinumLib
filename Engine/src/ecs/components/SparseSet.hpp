#pragma once
#include "../defintions.hpp"

// Interface for a component map
class ISparseSet
{
public:
    virtual ~ISparseSet() = default;
    virtual void OnEntityDestroyed(EntityID e) = 0;
};

template <typename T>
class SparseSet : public ISparseSet
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
        setDenseIndex(e, (uint16_t)dense.size());
        dense.push_back(std::move(component));
        denseToEntity.push_back(e);
        
    }

    // Check if entity e has a component of type T
    bool HasData(EntityID e)
    {
        return getDenseIndex(e) != NULL_INDEX;
    }

    // Get a pointer to the component of Type T for entity e
    T* GetData(EntityID e)
    {
        uint16_t index = getDenseIndex(e);
        if (index == NULL_INDEX)
        {
            fprintf(stderr, "[ERROR]: Entity %u does not have component of type: %s\n", e, typeid(T).name());
            return nullptr;
        }
        
        return &dense[index];
    }

    // Remove a component of Type T for entity e
    void RemoveData(EntityID e)
    {
        uint16_t index = getDenseIndex(e);
        if (index == NULL_INDEX)
        {
            fprintf(stderr, "[ERROR]: Entity %u does not have component of type: %s\n", e, typeid(T).name());
            return;
        }

        // Entity of the last dense element
        EntityID lastEntity = denseToEntity.back();

        // Swap the element to be deleted with the last element
        std::swap(dense[index], dense.back());
        std::swap(denseToEntity[index], denseToEntity.back());

        // Update the index for entities
        setDenseIndex(lastEntity, index);
        setDenseIndex(e, NULL_INDEX);

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
    std::vector<std::vector<uint16_t>> sparsePages; 
    std::vector<T> dense; 
    
    std::vector<EntityID> denseToEntity; 
   
    // Map an index of a component to an entity 
    void setDenseIndex(EntityID e, uint16_t index)
    {
        uint16_t page = e / MAX_PAGE_SIZE;
        uint16_t pageIndex = e % MAX_PAGE_SIZE;

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
    uint16_t getDenseIndex(EntityID e)
    {
        uint16_t page = e / MAX_PAGE_SIZE;
        uint16_t pageIndex = e % MAX_PAGE_SIZE;

        // Check if the page exists and the entity has a valid index
        if (page < sparsePages.size())
        {
            return sparsePages[page][pageIndex];
        }

        // Return NULL_INDEX if the entity doesn't have a component
        return NULL_INDEX;
    }
};