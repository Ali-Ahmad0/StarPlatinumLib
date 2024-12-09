#include "CollisionSystem.hpp"

void CollisionSystem::update()
{
    for (auto it1 = entities.begin(); it1 != entities.end(); ++it1)
    {
        EntityID eA = *it1;
        auto* boxA = ECS::GetComponent<AABB>(eA);
        auto* transformA = ECS::GetComponent<Transform>(eA);

        Vector2 centerA = boxA->center * (float)transformA->scale;
        centerA += transformA->position;

        Vector2 dimensionsAHalved = boxA->dimensions * (float)transformA->scale / 2;

        // Calculate new boundaries for box1
        Vector2 minA = centerA - dimensionsAHalved;
        Vector2 maxA = centerA + dimensionsAHalved;

        boxA->min = minA;
        boxA->max = maxA;

        for (auto it2 = std::next(it1); it2 != entities.end(); ++it2)
        {
            EntityID eB = *it2;

            auto* boxB = ECS::GetComponent<AABB>(eB);
            auto* transformB = ECS::GetComponent<Transform>(eB);

            Vector2 centerB = boxB->center * (float)transformB->scale;
            centerB += transformB->position;

            Vector2 dimensionsBHalved = boxB->dimensions * (float)transformB->scale / 2;

            // Calculate new boundaries for box2
            Vector2 minB = centerB - dimensionsBHalved;
            Vector2 maxB = centerB + dimensionsBHalved;
            
            boxB->min = minB;
            boxB->max = maxB;

            // Check for collisions between 2 bounding boxes
            if (boxA->intersects(*boxB))
            {
                // Resolve collisions
                resolve(boxA, transformA, boxB, transformB);
            }

            
        }
    }
}

void CollisionSystem::resolve(AABB* box1, Transform* t1, AABB* box2, Transform* t2)
{
    // Seperate colliding boxes
    if (box1->isSolid && box2->isSolid)
    {
        // Calculate overlap on the x and y axes
        float overlapX = std::min(box1->max.x - box2->min.x, box2->max.x - box1->min.x);
        float overlapY = std::min(box1->max.y - box2->min.y, box2->max.y - box1->min.y);

        int divideFactor1 = box1->isRigid ? 2 : 1;
        int divideFactor2 = box2->isRigid ? 2 : 1;

        // Find the axis with the least overlap and move the entities along that axis
        if (overlapX < overlapY)
        {
            // Move the entities apart along the x-axis
            if (t1->position.x < t2->position.x)
            {
                if (box1->isRigid)
                {
                    t1->position.x -= overlapX / divideFactor2;
                }
                if (box2->isRigid)
                {
                    t2->position.x += overlapX / divideFactor1;
                }
            }
            else
            {
                if (box1->isRigid)
                {
                    t1->position.x += overlapX / divideFactor2;
                }
                if (box2->isRigid)
                {
                    t2->position.x -= overlapX / divideFactor1;
                }
            }
        }
        else
        {
            // Move the entities apart along the y-axis
            if (t1->position.y < t2->position.y)
            {
                if (box1->isRigid)
                {
                    t1->position.y -= overlapY / divideFactor2;
                }
                if (box2->isRigid)
                {
                    t2->position.y += overlapY / divideFactor1;
                }
            }
            else
            {
                if (box1->isRigid)
                {
                    t1->position.y += overlapY / divideFactor2;
                }
                if (box2->isRigid)
                {
                    t2->position.y -= overlapY / divideFactor1;
                }
            }
        }
    }

}
