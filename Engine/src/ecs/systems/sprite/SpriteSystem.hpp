#pragma once

#include "../BaseSystem.hpp"

struct SpriteSystem : BaseSystem
{
    void Update(double delta = 0) override;

    void OnEntityAdded(EntityID e) override;
    void OnEntityRemoved(EntityID e) override;
    
private:
    void sortZ();
    std::vector<EntityID> m_entities;
};


