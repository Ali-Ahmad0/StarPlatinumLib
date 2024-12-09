#pragma once

#include "BaseSystem.hpp"
#include "../ECS.hpp"

struct CollisionSystem : public BaseSystem
{
    void update();
    void resolve(AABB* box1, Transform* t1, AABB* box2, Transform* t2);
};