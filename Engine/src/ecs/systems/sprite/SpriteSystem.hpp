#pragma once

#include "../ISystem.hpp"

struct SpriteSystem : ISystem
{
    void Update(double delta = 0) override;

    void OnEntityAdded(EntityID e) override;
    void OnEntityRemoved(EntityID e) override;
    void OnAllEntitiesRemoved() override;
    
private:
    void sortZ();
    std::vector<EntityID> m_entities;
};