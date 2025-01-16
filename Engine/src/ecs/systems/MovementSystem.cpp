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

void MovementSystem::onEntityAdded(EntityID e)
{
	// Add the entity
	entities.push_back(e);
}

void MovementSystem::onEntityRemoved(EntityID e)
{
	// Find and delete entity
	auto position = std::find(entities.begin(), entities.end(), e);
	entities.erase(position);
}