#pragma once
#include "../ECS.hpp"
#include "BaseSystem.hpp"

#include <iostream>

struct MovementSystem : public BaseSystem
{
	void update(ECS& ecs, double delta) 
	{
		for (EntityID e : entities) 
		{
			auto* transform = ecs.GetComponent<Transform>(e);
			auto* movement = ecs.GetComponent<Movement>(e);
			
			// Update transform component based on movemenet component
			transform->position = 
				transform->position.add(movement->direction.multiply(movement->scale).multiply((float)delta));
		}
	}
};