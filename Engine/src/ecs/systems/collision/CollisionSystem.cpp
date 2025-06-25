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

        auto* transformA = ECS::GetComponent<Transform>(entityA);
        auto* colliderA = ECS::GetComponent<Collider>(entityA);

        Vector2 newCenterA = Vector2::ZERO;

        // Update collider transform, rotation and flags
        if (colliderA->getShape() == ShapeType::CIRCLE) 
        {
            newCenterA = transformA->position + colliderA->getRadius();
        }
        else
        {
            float newX = transformA->position.x + colliderA->getWidth();
            float newY = transformA->position.y + colliderA->getHeight();

            newCenterA = Vector2(newX, newY);
        }

        if (newCenterA != colliderA->center || transformA->rotation != colliderA->rotation) 
        {
            colliderA->setUpdateRequired();
        }

        colliderA->center = newCenterA;
        colliderA->rotation = transformA->rotation;

        AABB* boxA = &colliderA->getAABB();

        edge.x = edge.isLeft ? boxA->min.x : boxA->max.x;

        if (edge.isLeft)
        {
            // Handle for all touching entities
            for (EntityID entityB : touching) 
            {
                // Get components for other entity
                auto* transformB = ECS::GetComponent<Transform>(entityB);
                auto* colliderB = ECS::GetComponent<Collider>(entityB);

                Vector2 newCenterB = Vector2::ZERO;

                // Update collider transform, rotation and flags
                if (colliderB->getShape() == ShapeType::CIRCLE)
                {
                    newCenterB = transformB->position + colliderB->getRadius();
                }
                else
                {
                    float newX = transformB->position.x + colliderB->getWidth();
                    float newY = transformB->position.y + colliderB->getHeight();

                    newCenterB = Vector2(newX, newY);
                }

                if (newCenterB != colliderB->center || transformB->rotation != colliderB->rotation)
                {
                    colliderB->setUpdateRequired();
                }

                colliderB->center = newCenterB;
                colliderB->rotation = transformB->rotation;

                AABB* boxB = &colliderB->getAABB();

                // FIrst: Check for AABB intersection
                if (boxA->checkIntersect(*boxB))
                {
                    // Case 1: Check for circle - circle collision
                    if (colliderA->getShape() == ShapeType::CIRCLE
                        && colliderB->getShape() == ShapeType::CIRCLE)
                    {
                        // Check for precise collision using radii
                        float distanceSquared = Vector2::magnitudeSquared(
                            colliderB->center - colliderA->center
                        );
                        float totalRadiiSquared = 
                            (colliderA->getRadius() + colliderB->getRadius()) * 
                            (colliderA->getRadius() + colliderB->getRadius());
                        
                        // Collision occurred
                        if (distanceSquared < totalRadiiSquared) 
                        {

                        }

                        break;
                    }

                    // Case 2: Check for box - box collision (SAT)
                    if (colliderA->getShape() == ShapeType::BOX
                        && colliderB->getShape() == ShapeType::BOX) 
                    {
                        break;
                    }

                    // Case 3: Check for circle - box collision (SAT)
                    if ((colliderA->getShape() == ShapeType::CIRCLE && colliderB->getShape() == ShapeType::BOX)
                        || (colliderA->getShape() == ShapeType::BOX && colliderB->getShape() == ShapeType::CIRCLE)) 
                    {
                        break;
                    }

                    // Resolve collisions
                    //resolve(boxA, transformA, boxB, transformB);
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

//void CollisionSystem::resolve(AABB* boxA, Transform* transformA, AABB* boxB, Transform* transformB)
//{
//    // Seperate colliding boxes
//    if (boxA->isSolid && boxB->isSolid)
//    {
//        // Calculate overlap on the x and y axes
//        float overlapX = std::min(boxA->max.x - boxB->min.x, boxB->max.x - boxA->min.x);
//        float overlapY = std::min(boxA->max.y - boxB->min.y, boxB->max.y - boxA->min.y);
//
//        int divideFactor1 = boxA->isRigid ? 2 : 1;
//        int divideFactor2 = boxB->isRigid ? 2 : 1;
//
//        // Find the axis with the least overlap and move the entities along that axis
//        if (overlapX < overlapY)
//        {
//            // Move the entities apart along the x-axis
//            if (transformA->position.x < transformB->position.x)
//            {
//                if (boxA->isRigid)
//                {
//                    transformA->position.x -= overlapX / divideFactor2;
//                }
//                if (boxB->isRigid)
//                {
//                    transformB->position.x += overlapX / divideFactor1;
//                }
//            }
//            else
//            {
//                if (boxA->isRigid)
//                {
//                    transformA->position.x += overlapX / divideFactor2;
//                }
//                if (boxB->isRigid)
//                {
//                    transformB->position.x -= overlapX / divideFactor1;
//                }
//            }
//        }
//        else
//        {
//            // Move the entities apart along the y-axis
//            if (transformA->position.y < transformB->position.y)
//            {
//                if (boxA->isRigid)
//                {
//                    transformA->position.y -= overlapY / divideFactor2;
//                }
//                if (boxB->isRigid)
//                {
//                    transformB->position.y += overlapY / divideFactor1;
//                }
//            }
//            else
//            {
//                if (boxA->isRigid)
//                {
//                    transformA->position.y += overlapY / divideFactor2;
//                }
//                if (boxB->isRigid)
//                {
//                    transformB->position.y -= overlapY / divideFactor1;
//                }
//            }
//        }
//    }
//
//}

void CollisionSystem::onEntityAdded(EntityID e)
{
    Collider* collider = ECS::GetComponent<Collider>(e);

    Edge edge1 = { e, collider->getAABB().min.x,  true };
    Edge edge2 = { e, collider->getAABB().max.x, false };

    edges.push_back(edge1);
    edges.push_back(edge2);
}

void CollisionSystem::onEntityRemoved(EntityID e)
{
    // Find and delete edges for entity
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [e](const Edge& edge) { return edge.entity == e; }), edges.end());
}
