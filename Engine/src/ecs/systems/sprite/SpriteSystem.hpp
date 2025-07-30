#pragma once

#include "../BaseSystem.hpp"

struct SpriteSystem : BaseSystem
{
    void update(double delta = 0) override;

    void onEntityAdded(EntityID e) override;
    void onEntityRemoved(EntityID e) override;
    
private:
    void sortZ();
    std::vector<EntityID> entities;
};


