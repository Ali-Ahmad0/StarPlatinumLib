#pragma once

#include "BaseSystem.hpp"
#include "../ECS.hpp"

struct CollisionSystem : public BaseSystem
{
    std::vector<EntityID> entities;

    void sortAABB();
    void update();
    void resolve(AABB* box1, Transform* t1, AABB* box2, Transform* t2);

    void onEntityAdded(EntityID e) override;
    void onEntityRemoved(EntityID e) override;
};