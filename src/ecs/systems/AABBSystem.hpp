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
			box1->min = t1->position.subtract(box1->dimensions.multiply((float)t1->scale).divide(2));
			box1->max = t1->position.add(box1->dimensions.multiply((float)t1->scale).divide(2));

			for (auto it2 = std::next(it1); it2 != entities.end(); ++it2)
			{
				EntityID e2 = *it2;
				
				AABB* box2 = ecs.GetComponent<AABB>(e2);
				Transform* t2 = ecs.GetComponent<Transform>(e2);

				// Calculate new boundaries for box2
				box2->min = t2->position.subtract(box2->dimensions.multiply((float)t2->scale).divide(2));
				box2->max = t2->position.add(box2->dimensions.multiply((float)t2->scale).divide(2));

				// Check for collisions between 2 bounding boxes
				box1->check(*box2);

				// Resolve collision
				if (box1->colliding() && box2->colliding()) 
				{
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
								t1->position.x -= overlapX / 2;
								t2->position.x += overlapX / 2;
							}
							else
							{
								t1->position.x += overlapX / 2;
								t2->position.x -= overlapX / 2;
							}
						}

						else
						{
							// Move the entities apart along the y-axis
							if (t1->position.y < t2->position.y)
							{
								t1->position.y -= overlapY / 2;
								t2->position.y += overlapY / 2;
							}
							else
							{
								t1->position.y += overlapY / 2;
								t2->position.y -= overlapY / 2;
							}
						}
					}
				}
			}
		}
	}
};