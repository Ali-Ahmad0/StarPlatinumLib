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

        if (Debug::showCollisionShapes) 
        {
            switch (colliderA->GetShape()) 
            {
            
            // Draw circle
            case ShapeType::CIRCLE:
                Debug::DrawCircle(
                    transformA->position + colliderA->centerOffset * (float)transformA->scale,
                    colliderA->GetRadius() * transformA->scale, colliderA->debugDrawColor
                );
                break;

            // Draw box
            case ShapeType::BOX:
                Debug::DrawRect(
                    transformA->position + colliderA->centerOffset * (float)transformA->scale,
                    colliderA->GetWidth() * (float)transformA->scale,
                    colliderA->GetHeight() * (float)transformA->scale, 
                    transformA->rotation, colliderA->debugDrawColor
                );
                break;

            }
        }

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

                AABB* boxB = colliderB->GetAABB(transformB);

                // First: Check for AABB intersection
                if (boxA->IsIntersecting(*boxB))
                {
                    Vector2 normal = Vector2::ZERO;
                    float depth = 0.0f;
                    bool isColliding = false;

                    // Case 1: Check for circle - circle collision
                    if (colliderA->GetShape() == ShapeType::CIRCLE && colliderB->GetShape() == ShapeType::CIRCLE)
                    {
                        isColliding = checkCircleCircleCollision(
                            transformA, transformB, colliderA, colliderB, &normal, &depth
                        );
                    }

                    // Case 2: Check for box - box collision (SAT)
                    else if (colliderA->GetShape() == ShapeType::BOX && colliderB->GetShape() == ShapeType::BOX)
                    {
                        isColliding = checkBoxBoxCollision(
                            transformA, transformB, colliderA, colliderB, &normal, &depth
                        );
                    }

                    // Case 3: Check for circle - box or box - circle collision (SAT)
                    else
                    {
                        isColliding = checkCircleBoxCollision(
                            transformA, transformB, colliderA, colliderB, &normal, &depth
                        );
                    }

                    if (isColliding) 
                    {
                        // Set collider properties and apply seperation
                        colliderA->isColliding = true;
                        colliderB->isColliding = true;

                        colliderA->normal = -normal;
                        colliderB->normal = normal;

                        seperateBodies(transformA, transformB, normal, depth, colliderA->isStatic, colliderB->isStatic);
                    }

                    else 
                    {
                        // Reset collider properties otherwise
                        colliderA->isColliding = false;
                        colliderB->isColliding = false;

                        colliderA->normal = Vector2::ZERO;
                        colliderB->normal = Vector2::ZERO;
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

bool CollisionSystem::checkCircleCircleCollision(Transform* transformA, Transform* transformB, Collider* colliderA, Collider* colliderB, Vector2* normal, float* depth)
{
    *normal = Vector2::ZERO;
    *depth = 0.0f;

    Vector2 centerA = transformA->position;
    Vector2 centerB = transformB->position;

    centerA += Vector2::Multiply(colliderA->centerOffset, (float)transformA->scale);
    centerB += Vector2::Multiply(colliderB->centerOffset, (float)transformB->scale);

    float radiusA = colliderA->GetRadius() * transformA->scale;
    float radiusB = colliderB->GetRadius() * transformB->scale;

    float distance = Vector2::Magnitude(centerB - centerA);
    float totalRadii = radiusA + radiusB;

    *normal = Vector2::Normalize(centerB - centerA);
    *depth = totalRadii - distance;

    // Collision occurs if distance between circles is less than the sum of their radii
    return distance < totalRadii;
}

bool CollisionSystem::checkBoxBoxCollision(Transform* transformA, Transform* transformB, Collider* colliderA, Collider* colliderB, Vector2* normal, float* depth)
{
    *normal = Vector2::ZERO;
    *depth = (float)INFINITY;

    Vector2 centerA = transformA->position;
    Vector2 centerB = transformB->position;

    centerA += Vector2::Multiply(colliderA->centerOffset, (float)transformA->scale);
    centerB += Vector2::Multiply(colliderB->centerOffset, (float)transformB->scale);

    auto& verticesA = colliderA->GetTransformedVertices(transformA);
    auto& verticesB = colliderB->GetTransformedVertices(transformB);

    // Polygon A
    for (size_t i = 0; i < verticesA.size(); i++) 
    {
        // Get 2 vertices from polygon A
        Vector2 vertex1 = verticesA[i];
        Vector2 vertex2 = verticesA[(i + 1) % verticesA.size()];

        // Edge from vertex 2 to vertex 1
        Vector2 edge = vertex2 - vertex1;

        // Normalized perpendicular to the edge
        Vector2 axis = Vector2(-edge.y, edge.x);
        axis = Vector2::Normalize(axis);

        // Project vertices onto the axis
        float minA, maxA, minB, maxB;
        projectVertices(verticesA, axis, &minA, &maxA);
        projectVertices(verticesB, axis, &minB, &maxB);

        // No overlap, no collision
        if (minA >= maxB || minB >= maxA) return false;

        float axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < *depth) 
        {
            *depth = axisDepth;
            *normal = axis;
        }
    }

    // Polygon B
    for (size_t i = 0; i < verticesB.size(); i++) 
    {
        // Get 2 vertices from polygon B
        Vector2 vertex1 = verticesB[i];
        Vector2 vertex2 = verticesB[(i + 1) % verticesB.size()];

        // Edge from vertex 2 to vertex 1
        Vector2 edge = vertex2 - vertex1;

        // Normalized perpendicular to the edge
        Vector2 axis = Vector2(-edge.y, edge.x);
        axis = Vector2::Normalize(axis);

        // Project vertices onto the axis
        float minA, maxA, minB, maxB;
        projectVertices(verticesA, axis, &minA, &maxA);
        projectVertices(verticesB, axis, &minB, &maxB);

        // No overlap, no collision
        if (minA >= maxB || minB >= maxA) return false;

        float axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < *depth)
        {
            *depth = axisDepth;
            *normal = axis;
        }
    }

    // Make sure normal always faces in the direction of centerB to centerA
    Vector2 direction = centerB - centerA;
    *normal = Vector2::Dot(direction, *normal) < 0.0f ? -*normal : *normal;

    return true;
}

bool CollisionSystem::checkCircleBoxCollision(Transform* transformA, Transform* transformB, Collider* colliderA, Collider* colliderB, Vector2* normal, float* depth)
{
    *normal = Vector2::ZERO;
    *depth = (float)INFINITY;

    Vector2 centerC = Vector2::ZERO; // Polygon center
    Vector2 centerP = Vector2::ZERO; //  Circle center
    
    std::array<Vector2, 4> vertices; // Polygon vertices
    float radius = 0.0f;             // Circle radius

    if (colliderA->GetShape() == ShapeType::BOX) 
    {
        centerP = transformA->position;
        centerC = transformB->position;

        centerP += Vector2::Multiply(colliderA->centerOffset, (float)transformA->scale);
        centerC += Vector2::Multiply(colliderB->centerOffset, (float)transformB->scale);

        radius = colliderB->GetRadius();

        vertices = colliderA->GetTransformedVertices(transformA);
    }

    else 
    {
        centerP = transformB->position;
        centerC = transformA->position;

        centerP += Vector2::Multiply(colliderB->centerOffset, (float)transformB->scale);
        centerC += Vector2::Multiply(colliderA->centerOffset, (float)transformA->scale);

        radius = colliderA->GetRadius();

        vertices = colliderB->GetTransformedVertices(transformB);
    }

    Vector2 axis = Vector2::ZERO;
    float axisDepth = 0.0f;

    float minA, maxA, minB, maxB;

    // Polygon
    for (size_t i = 0; i < vertices.size(); i++) 
    {
        // Get 2 vertices from polygon
        Vector2 vertex1 = vertices[i];
        Vector2 vertex2 = vertices[(i + 1) % vertices.size()];

        // Edge from vertex 2 to vertex 1
        Vector2 edge = vertex2 - vertex1;

        // Normalized axis perpendicular to edge
        axis = Vector2(-edge.y, edge.x);
        axis = Vector2::Normalize(axis);

        // Project vertices and circle onto the axis
        projectVertices(vertices, axis, &minA, &maxA);
        projectCircle(centerC, radius, axis, &minB, &maxB);

        // No overlap, no collision
        if (minA >= maxB || minB >= maxA) return false;

        axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < *depth) 
        {
            *depth = axisDepth;
            *normal = axis;
        }
    }

    // Get closest vertex to circle center
    Vector2 closestVertex = vertices[findClosestVertex(centerC, vertices)];
    
    // Normalized axis from vertex to circle center
    axis = closestVertex - centerC;
    axis = Vector2::Normalize(axis);

    // Project vertices and circle onto the axis
    projectVertices(vertices, axis, &minA, &maxA);
    projectCircle(centerC, radius, axis, &minB, &maxB);

    // No overlap, no collision
    if (minA >= maxB || minB >= maxA) return false;

    axisDepth = std::min(maxB - minA, maxA - minB);
    if (axisDepth < *depth)
    {
        *depth = axisDepth;
        *normal = axis;
    }
 
    // Make sure normal always faces in the direction of centerB to centerA
    Vector2 direction = colliderA->GetShape() == ShapeType::BOX 
        ? centerC - centerP 
        : centerP - centerC;

    *normal = Vector2::Dot(direction, *normal) < 0.0f ? -*normal : *normal;

    return true;
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
    *min =  (float)INFINITY;
    *max = -(float)INFINITY;

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

void CollisionSystem::seperateBodies(Transform* transformA, Transform* transformB, const Vector2& normal, float depth, bool isStaticA, bool isStaticB)
{
    // Case 1: Both objects are static - no resolution needed
    if (isStaticA && isStaticB)
    {
        return;
    }

    Vector2 seperation = Vector2::Multiply(normal, depth);
    

    // Case 2: One object is static, the other is dynamic
    if (isStaticA)
    {
        // Move transformB away by the full separation
        transformB->position += seperation;
    }
    else if (isStaticB)
    {
        // Move transformA away by the full separation
        transformA->position -= seperation;
    }
    // Case 3: Both objects are dynamic, separate them
    else
    {
        // Move each object by half the separation
        Vector2 halfSeparation = seperation * 0.5f;
        transformB->position += halfSeparation;
        transformA->position -= halfSeparation;
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