#pragma once

#include "BaseSystem.hpp"
#include "../ECS.hpp"

struct SpriteSystem : BaseSystem
{
    void sortZ();
    void update();
};


