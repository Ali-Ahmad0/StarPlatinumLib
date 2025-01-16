#pragma once

#include "BaseSystem.hpp"
#include "../ECS.hpp"

struct SpriteSystem : BaseSystem
{
    std::vector<EntityID> entities;

    void sortZ();
    void update();

    void onEntityAdded(EntityID e) override;
    void onEntityRemoved(EntityID e) override;
};


