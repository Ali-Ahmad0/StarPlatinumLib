#include "MovementSystem.hpp"
#include "../../ECS.hpp"

void MovementSystem::Update(double delta) 
{
	for (EntityID e : m_entities)
	{
		auto* transform = ECS::GetComponent<Transform>(e);
		auto* movement = ECS::GetComponent<Movement>(e);

		// Update transform component based on movemenet component
		transform->position.x += movement->direction.x * movement->speed.x * (float)delta;
		transform->position.y += movement->direction.y * movement->speed.y * (float)delta;
	}
}

void MovementSystem::OnEntityAdded(EntityID e)
{
	// Add the entity
	m_entities.push_back(e);
}

void MovementSystem::OnEntityRemoved(EntityID e)
{
	// Find and delete entity
	auto position = std::find(m_entities.begin(), m_entities.end(), e);
	m_entities.erase(position);
}