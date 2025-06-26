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

        AABB* boxA = colliderA->getAABB();

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

                AABB* boxB = colliderB->getAABB();

                // FIrst: Check for AABB intersection
                if (boxA->checkIntersect(*boxB))
                {
                    // Case 1: Check for circle - circle collision
                    if (colliderA->getShape() == ShapeType::CIRCLE
                        && colliderB->getShape() == ShapeType::CIRCLE)
                    {
                        // Check for precise collision using radii
                        float distance = Vector2::magnitude(
                            colliderB->center - colliderA->center
                        );
                        
                        float totalRadii = 
                            (colliderA->getRadius() + colliderB->getRadius());
                        
                        // Calculate collision normal and depth
                        Vector2 normal = distance > 0 
                            ? Vector2::normalize(colliderB->center - colliderA->center) 
                            : Vector2::RIGHT;
                        float depth = totalRadii - distance;

                        // Collision occurred
                        bool isColliding = distance < totalRadii;
                        colliderA->isColliding = isColliding;
                        colliderB->isColliding = isColliding;

                        if (isColliding)
                        {
                            colliderA->normal = -normal;
                            colliderA->depth = depth;

                            colliderB->normal = normal;
                            colliderB->depth = depth;

                            std::cout << "Collision - Normal:  (" << normal.x << ", " << normal.y << ") | Depth: " << depth << "\n";
                        }

                        else 
                        {
                            colliderA->normal = Vector2::ZERO;
                            colliderB->normal = Vector2::ZERO;

                            colliderA->depth = 0;
                            colliderB->depth = 0;
                        }

                    }

                    // Case 2: Check for box - box collision (SAT)
                    else if (colliderA->getShape() == ShapeType::BOX 
                        && colliderB->getShape() == ShapeType::BOX) 
                    {
                        Vector2 normal = Vector2::ZERO;
                        float depth = (float)INFINITY;

                        // Min and max projections of polygons
                        float minA = (float)INFINITY;
                        float maxA = -(float)INFINITY;

                        float minB = (float)INFINITY;
                        float maxB = -(float)INFINITY;

                        bool isColliding = true;

                        std::array<Vector2, 4> verticesA = colliderA->getTransformedVertices();
                        std::array<Vector2, 4> verticesB = colliderB->getTransformedVertices();

                        // Polygon A
                        for (size_t i = 0; i < verticesA.size(); i++) 
                        {
                            // Get a pair of vertices from polygon A
                            Vector2 vertex1 = verticesA[i];
                            Vector2 vertex2 = verticesA[(i + 1) % verticesA.size()];

                            Vector2 edge = vertex2 - vertex1;

                            // Get the possible seperating axis
                            Vector2 axis = Vector2(-edge.y, edge.x);
                            axis = Vector2::normalize(axis);
                            
                            // Project the vertices onto the axis
                            projectVertices(verticesA, axis, &minA, &maxA);
                            projectVertices(verticesB, axis, &minB, &maxB);

                            // Seperating axis exists, no collision
                            if (minA >= maxB || minB >= maxA) 
                            {
                                isColliding = false;
                                break; 
                            }

                            float axisDepth = std::min(maxB - minA, maxA - minB);
                            if (axisDepth < depth) 
                            {
                                depth = axisDepth;
                                normal = axis;
                            }
                        }

                        // Polygon B
                        for (size_t i = 0; i < verticesB.size(); i++) 
                        {
                            if (!isColliding) break;

                            // Get a pair of vertices from polygon A
                            Vector2 vertex1 = verticesB[i];
                            Vector2 vertex2 = verticesB[(i + 1) % verticesB.size()];

                            Vector2 edge = vertex2 - vertex1;

                            // Get the possible seperating axis
                            Vector2 axis = Vector2(-edge.y, edge.x);
                            axis = Vector2::normalize(axis);

                            // Project the vertices onto the axis
                            projectVertices(verticesA, axis, &minA, &maxA);
                            projectVertices(verticesB, axis, &minB, &maxB);

                            // Seperating axis exists, no collision
                            if (minA >= maxB || minB >= maxA) 
                            {
                                isColliding = false;
                                break;
                            }

                            float axisDepth = std::min(maxB - minA, maxA - minB);
                            if (axisDepth < depth)
                            {
                                depth = axisDepth;
                                normal = axis;
                            }
                        }

                        colliderA->isColliding = isColliding;
                        colliderB->isColliding = isColliding;

                        if (isColliding) 
                        {
                            std::cout << "Collision!\n";

                            // Apply correction to collision normal
                            Vector2 direction = colliderB->center - colliderA->center;
                            if (direction == Vector2::ZERO)
                            {
                                normal = Vector2::RIGHT;
                            }
                            else if (Vector2::dot(direction, normal) < 0)
                            {
                                normal = -normal;
                            }

                            colliderA->normal = -normal;
                            colliderA->depth = depth;

                            colliderB->normal = normal;
                            colliderB->depth = depth;
                        }

                        else 
                        {
                            colliderA->normal = Vector2::ZERO;
                            colliderA->depth = 0;

                            colliderB->normal = Vector2::ZERO;
                            colliderB->depth = 0;
                        }
                    }

                    // Case 3: Check for circle - box collision (SAT)
                    else if ((colliderA->getShape() == ShapeType::CIRCLE && colliderB->getShape() == ShapeType::BOX)
                        || (colliderA->getShape() == ShapeType::BOX && colliderB->getShape() == ShapeType::CIRCLE)) 
                    {
                        // First shape is circle and second shape is box
                        if (colliderA->getShape() == ShapeType::CIRCLE && colliderB->getShape() == ShapeType::BOX) 
                        {
                            
                        }

                        // First shape is box and second shape is circle
                        else if (colliderA->getShape() == ShapeType::BOX && colliderB->getShape() == ShapeType::CIRCLE) 
                        {
                        
                        }
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

// Project vertices onto possible seperating axis to get min and max values
void CollisionSystem::projectVertices(const std::array<Vector2, 4>& vertices, const Vector2& axis, float* min, float* max) 
{
    for (auto& vertex : vertices) 
    {
        // Projection using dot product
        float projection = Vector2::dot(vertex, axis);

        if (projection < *min) *min = projection;
        if (projection > *max) *max = projection;
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

    Edge edge1 = { e, collider->getAABB()->min.x,  true };
    Edge edge2 = { e, collider->getAABB()->max.x, false };

    edges.push_back(edge1);
    edges.push_back(edge2);
}

void CollisionSystem::onEntityRemoved(EntityID e)
{
    // Find and delete edges for entity
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [e](const Edge& edge) { return edge.entity == e; }), edges.end());
}
