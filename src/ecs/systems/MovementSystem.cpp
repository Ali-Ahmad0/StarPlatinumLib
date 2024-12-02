#include "MovementSystem.hpp"
#include "../../main/Game.hpp"

void MovementSystem::update(double delta) 
{
	for (EntityID e : entities)
	{
		auto* transform = ECS::GetComponent<Transform>(e);
		auto* movement = ECS::GetComponent<Movement>(e);

		// Update transform component based on movemenet component
		transform->position.x += movement->direction.x * movement->speed.x * (float)delta;
		transform->position.y += movement->direction.y * movement->speed.y * (float)delta;
	}
}