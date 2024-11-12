#pragma once
#include "../ECS.hpp"
#include "BaseSystem.hpp"

struct GravitySystem : public BaseSystem 
{
	void update(ECS& ecs, double delta) 
	{
		for (EntityID e : entities) 
		{
			auto* transform = ecs.GetComponent<Transform>(e);
			auto* gravity = ecs.GetComponent<Gravity>(e);

			// Calculate acceleration with direction
			Vector2 delVelocity;

			switch (gravity->direction)
			{
			case Direction::UP:
				delVelocity = Vector2(0, -gravity->g);
				break;

			case Direction::LEFT:
				delVelocity = Vector2(-gravity->g, 0);
				break;

			case Direction::DOWN:
				delVelocity = Vector2(0, gravity->g);
				break;

			case Direction::RIGHT:
				delVelocity = Vector2(gravity->g, 0);

			default:
				break;
			}

			// Apply gravity
			transform->position += delVelocity * delta;
		}
	}
};