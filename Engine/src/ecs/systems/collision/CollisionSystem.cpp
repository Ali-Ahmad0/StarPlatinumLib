#include "CollisionSystem.hpp"
#include "../../ECS.hpp"

void CollisionSystem::sortEdges()
{
    // Insertion sort on the edges
    for (size_t i = 1; i < edges.size(); i++)
    {
        Edge k = edges[i];
        int j = (int)i - 1;

        // Compare current edge with the previous
        while (j >= 0 && edges[j].x > k.x)
        {
            edges[j + 1] = edges[j]; 
            j--;
        }

        // Place the current in the correct position
        edges[j + 1] = k;
    }
}

void CollisionSystem::update()
{
    // Sort edges from left to right
    sortEdges();

    for (auto& edge : edges)
    {
        // Get the entity and components
        EntityID entityA = edge.entity;
        auto* boxA = ECS::GetComponent<AABB>(entityA);
        auto* transformA = ECS::GetComponent<Transform>(entityA);

        Vector2 centerA = boxA->center * (float)transformA->scale;
        centerA += transformA->position;

        Vector2 dimensionsAHalved = boxA->dimensions * (float)transformA->scale / 2;

        // Calculate new boundaries for boxA
        Vector2 minA = centerA - dimensionsAHalved;
        Vector2 maxA = centerA + dimensionsAHalved;

        boxA->min = minA;
        boxA->max = maxA;

        edge.x = edge.isLeft ? boxA->min.x : boxA->max.x;

        if (edge.isLeft)
        {
            // Handle for all touching entities
            for (EntityID entityB : touching) 
            {
                // Get components for other entity
                auto* transformB = ECS::GetComponent<Transform>(entityB);
                auto* boxB = ECS::GetComponent<AABB>(entityB);
                
                Vector2 centerB = boxB->center * (float)transformB->scale;
                centerB += transformB->position;

                Vector2 dimensionsBHalved = boxB->dimensions * (float)transformB->scale / 2;

                // Calculate new boundaries for box2
                Vector2 minB = centerB - dimensionsBHalved;
                Vector2 maxB = centerB + dimensionsBHalved;
                    
                boxB->min = minB;
                boxB->max = maxB;

                // Check for possible intersection
                if (boxA->intersects(*boxB))
                {
                    // Resolve collisions
                    resolve(boxA, transformA, boxB, transformB);
                }
            }

            touching.insert(edge.entity);
        }

        else 
        {
            touching.erase(edge.entity);
        }
    }
}

void CollisionSystem::resolve(AABB* boxA, Transform* transformA, AABB* boxB, Transform* transformB)
{
    // Seperate colliding boxes
    if (boxA->isSolid && boxB->isSolid)
    {
        // Calculate overlap on the x and y axes
        float overlapX = std::min(boxA->max.x - boxB->min.x, boxB->max.x - boxA->min.x);
        float overlapY = std::min(boxA->max.y - boxB->min.y, boxB->max.y - boxA->min.y);

        int divideFactor1 = boxA->isRigid ? 2 : 1;
        int divideFactor2 = boxB->isRigid ? 2 : 1;

        // Find the axis with the least overlap and move the entities along that axis
        if (overlapX < overlapY)
        {
            // Move the entities apart along the x-axis
            if (transformA->position.x < transformB->position.x)
            {
                if (boxA->isRigid)
                {
                    transformA->position.x -= overlapX / divideFactor2;
                }
                if (boxB->isRigid)
                {
                    transformB->position.x += overlapX / divideFactor1;
                }
            }
            else
            {
                if (boxA->isRigid)
                {
                    transformA->position.x += overlapX / divideFactor2;
                }
                if (boxB->isRigid)
                {
                    transformB->position.x -= overlapX / divideFactor1;
                }
            }
        }
        else
        {
            // Move the entities apart along the y-axis
            if (transformA->position.y < transformB->position.y)
            {
                if (boxA->isRigid)
                {
                    transformA->position.y -= overlapY / divideFactor2;
                }
                if (boxB->isRigid)
                {
                    transformB->position.y += overlapY / divideFactor1;
                }
            }
            else
            {
                if (boxA->isRigid)
                {
                    transformA->position.y += overlapY / divideFactor2;
                }
                if (boxB->isRigid)
                {
                    transformB->position.y -= overlapY / divideFactor1;
                }
            }
        }
    }

}

void CollisionSystem::onEntityAdded(EntityID e)
{
    AABB* aabb = ECS::GetComponent<AABB>(e);

    Edge edge1 = { e, aabb->min.x, true  };
    Edge edge2 = { e, aabb->max.x, false };

    edges.push_back(edge1);
    edges.push_back(edge2);
}

void CollisionSystem::onEntityRemoved(EntityID e)
{
    // Find and delete edges for entity
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [e](const Edge& edge) { return edge.entity == e; }), edges.end());
}
