#pragma once

#include "../ISystem.hpp"
#include "../../ECS.hpp"

struct SpriteSystem : ISystem
{
    void Update(double delta = 0) override;

    void OnEntityAdded(EntityID e) override;
    void OnEntityRemoved(EntityID e) override;
    void OnAllEntitiesRemoved() override;
    
    // Flag to toggle Y sort
    bool enableYSort = false;

private:
    void sortRenderingOrder();
    float getGlobalYSortOrigin(Transform* transform, Sprite* sprite);
    std::vector<EntityID> m_entities;
};