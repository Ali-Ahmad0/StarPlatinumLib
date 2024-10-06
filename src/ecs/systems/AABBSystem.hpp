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

			AABB* box1 = ecs.GetComponent<AABB>(e1);
			Transform* t1 = ecs.GetComponent<Transform>(e1);
			
			// Calculate new boundaries for box1
			box1->min = t1->position.subtract(box1->dimensions.multiply(t1->scale).divide(2));
			box1->max = t1->position.add(box1->dimensions.multiply(t1->scale).divide(2));

			for (auto it2 = std::next(it1); it2 != entities.end(); ++it2)
			{
				EntityID e2 = *it2;
				
				AABB* box2 = ecs.GetComponent<AABB>(e2);
				Transform* t2 = ecs.GetComponent<Transform>(e2);

				// Calculate new boundaries for box2
				box2->min = t2->position.subtract(box2->dimensions.multiply(t2->scale).divide(2));
				box2->max = t2->position.add(box2->dimensions.multiply(t2->scale).divide(2));

				// Check for collisions between 2 bounding boxes
				box1->check(*box2);
			}
		}
	}
};