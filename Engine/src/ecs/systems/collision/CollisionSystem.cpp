#include "CollisionSystem.hpp"
#include "../../ECS.hpp"
#include "../../../main/debug/Debug.hpp"

void CollisionSystem::sortEdges()
{
    // Insertion sort on the edges
    for (size_t i = 1; i < m_edges.size(); i++)
    {
        Edge k = m_edges[i];
        int j = (int)i - 1;

        // Compare current edge with the previous
        while (j >= 0 && m_edges[j].x > k.x)
        {
            m_edges[j + 1] = m_edges[j]; 
            j--;
        }

        // Place the current in the correct position
        m_edges[j + 1] = k;
    }
}

void CollisionSystem::Update(double delta)
{
    // Sort edges from left to right
    sortEdges();

    for (auto& edge : m_edges)
    {
        // Get the entity and components
        EntityID entityA = edge.entity;

        auto* transformA = ECS::GetComponent<Transform>(entityA);
        auto* colliderA = ECS::GetComponent<Collider>(entityA);

        Vector2 centerA = colliderA->centerOffset;
        centerA += transformA->position;

        AABB* boxA = colliderA->GetAABB(transformA);

        edge.x = edge.isLeft ? boxA->min.x : boxA->max.x;

        if (edge.isLeft)
        {
            // Handle for all touching entities
            for (EntityID entityB : m_touching)
            {
                // Get components for other entity
                auto* transformB = ECS::GetComponent<Transform>(entityB);
                auto* colliderB = ECS::GetComponent<Collider>(entityB);

                Vector2 centerB = colliderB->centerOffset;
                centerB += transformB->position;

                AABB* boxB = colliderB->GetAABB(transformB);

                // FIrst: Check for AABB intersection
                if (boxA->IsIntersecting(*boxB))
                {
                    // Case 1: Check for circle - circle collision
                    if (colliderA->GetShape() == ShapeType::CIRCLE && colliderB->GetShape() == ShapeType::CIRCLE)
                    {
                        // Check for precise collision using radii
                        float distance = Vector2::Magnitude(centerB - centerA);

                        float totalRadii =
                            (colliderA->GetRadius() + colliderB->GetRadius());

                        // Calculate collision normal and depth
                        Vector2 normal = distance > 0
                            ? Vector2::Normalize(centerB - centerA)
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

                            // Resolve the collisions
                            if (colliderA->isSolid && colliderB->isSolid)
                            {
                                separate(transformA, transformB, normal * depth, colliderA->isStatic, colliderB->isStatic);
                            }
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
                    else if (colliderA->GetShape() == ShapeType::BOX && colliderB->GetShape() == ShapeType::BOX)
                    {
                        // Collision data
                        Vector2 normal = Vector2::ZERO;
                        float depth = (float)INFINITY;

                        bool isColliding = true;

                        std::array<Vector2, 4> verticesA = colliderA->GetTransformedVertices(transformA);
                        std::array<Vector2, 4> verticesB = colliderB->GetTransformedVertices(transformB);

                        // Polygon A
                        for (size_t i = 0; i < verticesA.size(); i++)
                        {
                            // Get a pair of vertices from polygon A
                            Vector2 vertex1 = verticesA[i];
                            Vector2 vertex2 = verticesA[(i + 1) % verticesA.size()];

                            Vector2 edge = vertex2 - vertex1;

                            // Get the axis
                            Vector2 axis = Vector2(-edge.y, edge.x);
                            axis = Vector2::Normalize(axis);

                            // Project the vertices onto the axis
                            float minA = (float)INFINITY;
                            float maxA = -(float)INFINITY;

                            float minB = (float)INFINITY;
                            float maxB = -(float)INFINITY;

                            projectVertices(verticesA, axis, &minA, &maxA);
                            projectVertices(verticesB, axis, &minB, &maxB);

                            // Seperation, no collision
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

                            // Get the axis
                            Vector2 axis = Vector2(-edge.y, edge.x);
                            axis = Vector2::Normalize(axis);

                            // Project the vertices onto the axis
                            float minA = (float)INFINITY;
                            float maxA = -(float)INFINITY;

                            float minB = (float)INFINITY;
                            float maxB = -(float)INFINITY;
                            projectVertices(verticesA, axis, &minA, &maxA);
                            projectVertices(verticesB, axis, &minB, &maxB);

                            // Seperation, no collision
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
                            // Apply correction to collision normal
                            Vector2 direction = centerB - centerA;
                            if (direction == Vector2::ZERO)
                            {
                                normal = Vector2::RIGHT;
                            }
                            else if (Vector2::Dot(direction, normal) < 0)
                            {
                                normal = -normal;
                            }

                            colliderA->normal = -normal;
                            colliderA->depth = depth;

                            colliderB->normal = normal;
                            colliderB->depth = depth;

                            // Resolve the collisions
                            if (colliderA->isSolid && colliderB->isSolid)
                            {
                                separate(transformA, transformB, normal * depth, colliderA->isStatic, colliderB->isStatic);
                            }
                        }

                        else
                        {
                            colliderA->normal = Vector2::ZERO;
                            colliderA->depth = 0;

                            colliderB->normal = Vector2::ZERO;
                            colliderB->depth = 0;
                        }
                    }

                    // Case 3: Check for circle - box or box - circle collision (SAT)
                    else
                    {
                        // Collision data
                        Vector2 normal = Vector2::ZERO;
                        float depth = (float)INFINITY;

                        bool isColliding = true;

                        Vector2 centerC; // Center of circle
                        Vector2 centerP; // Center of polyon

                        std::array<Vector2, 4> vertices;
                        float radius = 0;

                        // First shape is circle and second shape is box
                        if (colliderA->GetShape() == ShapeType::CIRCLE && colliderB->GetShape() == ShapeType::BOX)
                        {
                            centerC = centerA;
                            centerP = centerB;

                            radius = colliderA->GetRadius();
                            vertices = colliderB->GetTransformedVertices(transformB);
                        }

                        // First shape is box and second shape is circle
                        else if (colliderA->GetShape() == ShapeType::BOX && colliderB->GetShape() == ShapeType::CIRCLE)
                        {
                            centerC = centerB;
                            centerP = centerA;

                            radius = colliderB->GetRadius();
                            vertices = colliderA->GetTransformedVertices(transformA);
                        }

                        // Polygon
                        for (int i = 0; i < vertices.size(); i++)
                        {
                            // Get a pair of vertices from the polygon
                            Vector2 vertex1 = vertices[i];
                            Vector2 vertex2 = vertices[(i + 1) % vertices.size()];

                            Vector2 edge = vertex2 - vertex1;

                            // Get the axis
                            Vector2 axis = Vector2(-edge.y, edge.x);
                            axis = Vector2::Normalize(axis);

                            // Project the vertices and circle onto the axis
                            float minA = (float)INFINITY;
                            float maxA = -(float)INFINITY;

                            float minB = (float)INFINITY;
                            float maxB = -(float)INFINITY;
                            projectVertices(vertices, axis, &minA, &maxA);

                            projectCircle(centerC, radius, axis, &minB, &maxB);

                            // Seperation, no collision
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

                        if (isColliding)
                        {
                            // Get closest polygon vertex to center and compute axis
                            Vector2 closestVertex = vertices[findClosestVertex(centerC, vertices)];
                            Vector2 axis = Vector2::Normalize(closestVertex - centerC);

                            // Project vertices and circle onto this axis
                            float minA = (float)INFINITY;
                            float maxA = -(float)INFINITY;

                            float minB = (float)INFINITY;
                            float maxB = -(float)INFINITY;
                            projectVertices(vertices, axis, &minA, &maxA);

                            projectCircle(centerC, radius, axis, &minB, &maxB);

                            // Seperation, no collision
                            if (minA >= maxB || minB >= maxA)
                            {
                                isColliding = false;

                                colliderA->normal = Vector2::ZERO;
                                colliderA->depth = 0;

                                colliderB->normal = Vector2::ZERO;
                                colliderB->depth = 0;
                            }

                            // Collision occurred
                            else
                            {
                                float axisDepth = std::min(maxB - minA, maxA - minB);
                                if (axisDepth < depth)
                                {
                                    depth = axisDepth;
                                    normal = axis;
                                }

                                // Apply correction to collision normal
                                Vector2 direction = centerB - centerA;
                                if (direction == Vector2::ZERO)
                                {
                                    normal = Vector2::RIGHT;
                                }
                                else if (Vector2::Dot(direction, normal) < 0)
                                {
                                    normal = -normal;
                                }

                                colliderA->normal = -normal;
                                colliderA->depth = depth;

                                colliderB->normal = normal;
                                colliderB->depth = depth;

                                // Resolve the collisions
                                if (colliderA->isSolid && colliderB->isSolid)
                                {
                                    separate(transformA, transformB, normal * depth, colliderA->isStatic, colliderB->isStatic);
                                }
                            }
                        }

                        else
                        {
                            colliderA->normal = Vector2::ZERO;
                            colliderB->depth = 0;

                            colliderB->normal = Vector2::ZERO;
                            colliderB->depth = 0;
                        }

                        colliderA->isColliding = isColliding;
                        colliderB->isColliding = isColliding;

                    }
                }
            }

            m_touching.insert(edge.entity);
        }

        else
        {
            m_touching.erase(edge.entity);
        }
    }
}

// Returns the index of the vertex closest to circle center
size_t CollisionSystem::findClosestVertex(const Vector2& center, const std::array<Vector2, 4>& vertices)
{
    size_t index = std::numeric_limits<size_t>::max();
    float min = (float)INFINITY;

    for (int i = 0; i < vertices.size(); i++) 
    {
        // Get distance squared
        float distanceSquared = Vector2::MagnitudeSquared(Vector2::Subtract(center, vertices[i]));

        // Update minimum value
        if (distanceSquared < min) 
        {
            min = distanceSquared;
            index = i;
        }
    }

    return index;
}

// Project vertices onto possible seperating axis to get min and max values
void CollisionSystem::projectVertices(const std::array<Vector2, 4>& vertices, const Vector2& axis, float* min, float* max) 
{
    for (auto& vertex : vertices) 
    {
        // Projection using dot product
        float projection = Vector2::Dot(vertex, axis);

        if (projection < *min) *min = projection;
        if (projection > *max) *max = projection;
    }
}

void CollisionSystem::projectCircle(const Vector2& center, float radius, const Vector2& axis, float* min, float* max)
{
    /*
    Draw a diameter line perpendicular
    to the axis and then get the points
    on the circumference that the line
    intersects
    */
    Vector2 radiusVector = Vector2::Multiply(axis, radius);

    // Get points on both ends of the line
    Vector2 pointA = Vector2::Add(center, radiusVector);
    Vector2 pointB = Vector2::Subtract(center, radiusVector);

    // Projection using dot product
    *min = Vector2::Dot(pointA, axis);
    *max = Vector2::Dot(pointB, axis);

    if (*min > *max)
        std::swap(*min, *max);
}

void CollisionSystem::separate(Transform* transformA, Transform* transformB, const Vector2& separation, bool isStaticA, bool isStaticB)
{
    // Case 1: Both objects are static - no resolution needed
    if (isStaticA && isStaticB) 
    {
        return;
    }

    // Case 2: One object is static, the other is dynamic
    if (isStaticA) 
    {
        // Move transformB away by the full separation
        transformB->position += separation;
    }
    else if (isStaticB)
    {
        // Move transformA away by the full separation
        transformA->position -= separation;
    }
    // Case 3: Both objects are dynamic, seperate them
    else
    {
        // Move each object by half the separation
        Vector2 halfSeparation = Vector2::Multiply(separation, 0.5);
        transformA->position -= halfSeparation;
        transformB->position += halfSeparation;
    }
}

void CollisionSystem::OnEntityAdded(EntityID e)
{
    Transform* transform = ECS::GetComponent<Transform>(e);
    Collider* collider = ECS::GetComponent<Collider>(e);
    
    AABB* aabb = collider->GetAABB(transform);
    Edge edge1 = { e, aabb->min.x,  true };
    Edge edge2 = { e, aabb->max.x, false };

    m_edges.push_back(edge1);
    m_edges.push_back(edge2);
}

void CollisionSystem::OnEntityRemoved(EntityID e)
{
    // Find and delete edges for entity
    m_edges.erase(std::remove_if(m_edges.begin(), m_edges.end(),
        [e](const Edge& edge) { return edge.entity == e; }), m_edges.end());
}

void CollisionSystem::OnAllEntitiesRemoved()
{
    m_edges.clear();
    m_touching.clear();
}
