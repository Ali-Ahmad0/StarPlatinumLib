#include "CollisionSystem.hpp"

void CollisionSystem::sortAABB() 
{
    // Insertion sort entities from left to right
    for (size_t i = 1; i < entities.size(); i++)
    {
        EntityID key = entities[i];
        auto* boxA = ECS::GetComponent<AABB>(key);

        int j = (int)i - 1;
        auto* boxB = ECS::GetComponent<AABB>(entities[j]);

        while (j >= 0 && boxB->min.x > boxA->min.x)
        {
            entities[j + 1] = entities[j];
            j--;
        }
        entities[j + 1] = key;
    }
}

void CollisionSystem::update()
{
    // Sort entities from left to right
    sortAABB();

    for (size_t i = 0; i < entities.size(); i++)
    {
        EntityID entityA = entities[i];
        auto* boxA = ECS::GetComponent<AABB>(entityA);
        auto* transformA = ECS::GetComponent<Transform>(entityA);

        Vector2 centerA = boxA->center * (float)transformA->scale;
        centerA += transformA->position;

        Vector2 dimensionsAHalved = boxA->dimensions * (float)transformA->scale / 2;

        // Calculate new boundaries for box1
        Vector2 minA = centerA - dimensionsAHalved;
        Vector2 maxA = centerA + dimensionsAHalved;

        boxA->min = minA;
        boxA->max = maxA;

        for (size_t j = i + 1; j < entities.size(); j++)
        {
            EntityID entityB = entities[j];

            auto* boxB = ECS::GetComponent<AABB>(entityB);
            auto* transformB = ECS::GetComponent<Transform>(entityB);

            Vector2 centerB = boxB->center * (float)transformB->scale;
            centerB += transformB->position;

            Vector2 dimensionsBHalved = boxB->dimensions * (float)transformB->scale / 2;

            // Calculate new boundaries for box2
            Vector2 minB = centerB - dimensionsBHalved;
            Vector2 maxB = centerB + dimensionsBHalved;
            
            boxB->min = minB;
            boxB->max = maxB;

            // No chance of further collisions
            if (boxB->min.x > boxA->max.x) 
            {
                break;
            }

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
