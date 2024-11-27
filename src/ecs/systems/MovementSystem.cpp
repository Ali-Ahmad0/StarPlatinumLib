#include "MovementSystem.hpp"
#include "../../main/Game.hpp"

void MovementSystem::update(double delta) 
{
	for (EntityID e : entities)
	{
		auto* transform = ECS::GetComponent<Transform>(e);
		auto* movement = ECS::GetComponent<Movement>(e);

		// Update transform component based on movemenet component
		transform->position += movement->direction * movement->speed * (float)delta;
	}
}