#include "GravitySystem.hpp"

void GravitySystem::update(double delta)
{
	for (EntityID e : entities)
	{
		auto* transform = ECS::GetComponent<Transform>(e);
		auto* gravity = ECS::GetComponent<Gravity>(e);

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
		transform->position += delVelocity * (float)delta;
	}
}