#include "VIntegrationSystem.hpp"
#include "../../ECS.hpp"

#define METER 32 // Meter to pixel conversion

void VIntegrationSystem::update(double delta)
{
	for (const EntityID e : entities) 
	{
		auto* transform = ECS::GetComponent<Transform>(e);
		auto* verlet = ECS::GetComponent<VerletObject>(e);
		
		if (verlet->isStationary) continue;

		// Get properties to apply verlet integration
		Vector2 currPosition = transform->position;
		Vector2 prevPosition = verlet->prevPosition;

		Vector2 acceleration = verlet->acceleration;

		// Verlet integration
		Vector2 nextPosition = (currPosition * 2.0f) - prevPosition + (acceleration * METER * (float)(delta * delta));

		// Update positions
		verlet->prevPosition = currPosition;
		transform->position = nextPosition;

		// Reset acceleration for next frame
		verlet->acceleration = Vector2::ZERO;
	}
}

void VIntegrationSystem::onEntityAdded(EntityID e)
{
	// Add the entity
	entities.push_back(e);
}

void VIntegrationSystem::onEntityRemoved(EntityID e)
{
	// Find and delete entity
	auto position = std::find(entities.begin(), entities.end(), e);
	entities.erase(position);
}
