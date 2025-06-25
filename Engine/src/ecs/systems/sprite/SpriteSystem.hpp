#pragma once

#include "../BaseSystem.hpp"

struct SpriteSystem : BaseSystem
{
    std::vector<EntityID> entities;

    void update();

    void onEntityAdded(EntityID e) override;
    void onEntityRemoved(EntityID e) override;
    
private:
    void sortZ();
};


