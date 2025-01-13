#pragma once

#include "BaseSystem.hpp"
#include "../ECS.hpp"

struct SpriteSystem : BaseSystem
{
    void sortZ();
    void sortY();
    void update();

    bool useZSorting = false;
    bool useYSorting = false;
};


