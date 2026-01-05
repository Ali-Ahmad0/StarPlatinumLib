#pragma once

#include "../ISystem.hpp"
#include "../../../common/Components.hpp"

struct CollisionSystem : public ISystem
{
    void Update(double delta = 0) override;

    void OnEntityAdded(EntityID e) override;
    void OnEntityRemoved(EntityID e) override;
    void OnAllEntitiesRemoved() override;

private:
    struct Edge 
    {
        EntityID entity;
        float x;
        bool isLeft;

        Edge(EntityID entity, float x, bool isLeft) 
            : entity(entity), x(x), isLeft(isLeft) {}
    };

    // X axis edges for AABBs
    std::vector<Edge> m_edges;

    // Keep track of touching edges
    std::set<EntityID> m_touching;

    void sortEdges();
    
    bool checkCircleCircleCollision(
        Vector2& centerA, Vector2& centerB, float radiusA, float radiusB, Vector2* normal, float* depth
    );

    bool checkBoxBoxCollision(
        Vector2& centerA, Vector2& centerB,
        std::array<Vector2, 4>& verticesA, std::array<Vector2, 4>& verticesB, Vector2* normal, float* depth
    );

    bool checkCircleBoxCollision(
        Vector2& centerC, Vector2& centerP,
        float radius, std::array<Vector2, 4>& vertices, Vector2* normal, float* depth
    );

    size_t findClosestVertex(const Vector2& center, const std::array<Vector2, 4>& vertices);
    void projectVertices(const std::array<Vector2, 4>& vertices, const Vector2& axis, float* min, float* max);
    void projectCircle(const Vector2& center, float radius, const Vector2& axis, float* min, float* max);

    void seperateBodies(Transform* transformA, Transform* transformB, const Vector2& normal, float depth, bool isStaticA, bool isStaticB);
};