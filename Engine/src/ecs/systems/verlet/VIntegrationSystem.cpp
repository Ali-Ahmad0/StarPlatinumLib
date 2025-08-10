#include "VIntegrationSystem.hpp"
#include "../../ECS.hpp"

#define METER 32 // Meter to pixel conversion

void VIntegrationSystem::Update(double delta)
{
	for (const EntityID e : m_entities) 
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

void VIntegrationSystem::OnEntityAdded(EntityID e)
{
	// Add the entity
	m_entities.push_back(e);
}

void VIntegrationSystem::OnEntityRemoved(EntityID e)
{
	// Find and delete entity
	auto position = std::find(m_entities.begin(), m_entities.end(), e);
	m_entities.erase(position);
}
