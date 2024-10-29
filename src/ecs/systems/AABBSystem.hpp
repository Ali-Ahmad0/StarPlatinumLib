#pragma once

#include "BaseSystem.hpp"
#include "../ECS.hpp"

struct AABBSystem : public BaseSystem
{
	void update(ECS& ecs)
	{
		for (auto it1 = entities.begin(); it1 != entities.end(); ++it1)
		{
			EntityID e1 = *it1;

			AABB* box1 = ecs.GetComponent<AABB>(e1); Transform* t1 = ecs.GetComponent<Transform>(e1);

            Vector2 center1 = Vector2::multiply(box1->center, (float)t1->scale);
            center1 = center1.add(t1->position);
			
			// Calculate new boundaries for box1
            float minAx = center1.x - box1->dimensions.x * t1->scale / 2;
            float minAy = center1.y - box1->dimensions.y * t1->scale / 2;

            float maxAx = center1.x + box1->dimensions.x * t1->scale / 2;
            float maxAy = center1.y + box1->dimensions.y * t1->scale / 2;

            box1->min = Vector2(minAx, minAy);
            box1->max = Vector2(maxAx, maxAy);

            for (auto it2 = std::next(it1); it2 != entities.end(); ++it2)
            {
                EntityID e2 = *it2;

                AABB* box2 = ecs.GetComponent<AABB>(e2); Transform* t2 = ecs.GetComponent<Transform>(e2);

                Vector2 center2 = Vector2::multiply(box2->center, ((float)t2->scale));
                center2 = center2.add(t2->position);

                // Calculate new boundaries for box2
                float minBx = center2.x - box2->dimensions.x * t2->scale / 2;
                float minBy = center2.y - box2->dimensions.y * t2->scale / 2;

                float maxBx = center2.x + box2->dimensions.x * t2->scale / 2;
                float maxBy = center2.y + box2->dimensions.y * t2->scale / 2;

                box2->min = Vector2(minBx, minBy);
                box2->max = Vector2(maxBx, maxBy);

                // Check for collisions between 2 bounding boxes
                if (box1->intersects(*box2)) 
                {
                    // Resolve collisions
                    resolve(box1, t1, box2, t2);
                }
            }
		}
	}

    void resolve(AABB* box1, Transform* t1, AABB* box2, Transform* t2) 
    {
        // Seperate colliding boxes
        if (box1->isSolid && box2->isSolid)
        {
            // Calculate overlap on the x and y axes
            float overlapX = std::min(box1->max.x - box2->min.x, box2->max.x - box1->min.x);
            float overlapY = std::min(box1->max.y - box2->min.y, box2->max.y - box1->min.y);

            // Find the axis with the least overlap and move the entities along that axis
            if (overlapX < overlapY)
            {
                // Move the entities apart along the x-axis
                if (t1->position.x < t2->position.x)
                {
                    if (box1->isRigid)
                    {
                        t1->position.x -= overlapX / 2;
                    }
                    if (box2->isRigid)
                    {
                        t2->position.x += overlapX / 2;
                    }
                }
                else
                {
                    if (box1->isRigid)
                    {
                        t1->position.x += overlapX / 2;
                    }
                    if (box2->isRigid)
                    {
                        t2->position.x -= overlapX / 2;
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
                        t1->position.y -= overlapY / 2;
                    }
                    if (box2->isRigid)
                    {
                        t2->position.y += overlapY / 2;
                    }
                }
                else
                {
                    if (box1->isRigid)
                    {
                        t1->position.y += overlapY / 2;
                    }
                    if (box2->isRigid)
                    {
                        t2->position.y -= overlapY / 2;
                    }
                }
            }
        }
        
    }
};