#pragma once

#include "../BaseSystem.hpp"
#include "../../components/Components.hpp"

struct CollisionSystem : public BaseSystem
{

    void update();
    //void resolve(
    //    AABB* boxA, Transform* transformA, 
    //    AABB* boxB, Transform* transformB);

    void onEntityAdded(EntityID e) override;
    void onEntityRemoved(EntityID e) override;

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
    std::vector<Edge> edges;

    // Keep track of touching edges
    std::set<EntityID> touching;

    void sortEdges();
    void projectVertices(const std::array<Vector2, 4>& vertices, const Vector2& axis, float* min, float* max);
};