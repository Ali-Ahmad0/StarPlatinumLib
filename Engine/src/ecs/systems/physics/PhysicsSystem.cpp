#include "PhysicsSystem.hpp"

#include "../../ECS.hpp"
#include "../../../common/Utils.hpp"

void PhysicsSystem::update(double delta) 
{
	for (EntityID e : entities) 
	{
		PhysicsBody* physics = ECS::GetComponent<PhysicsBody>(e);

		// Skip moving static bodies
		if (physics->isStatic || physics->getAcceleration() == Vector2::ZERO) continue;

		Transform* transform = ECS::GetComponent<Transform>(e);

		Vector2 nextPos = Vector2::ZERO;
		Vector2 prevPos = physics->getPreviousPos();
		Vector2 currPos = transform->position;

		// Acceleration in meter per second squared
		Vector2 acceleration = physics->getAcceleration() * meter;

		// Verlet integration formula:
		// Next position = (current positon * 2) - previous position + (a * dt * dt)
		nextPos = (currPos * 2) - prevPos + (acceleration * (float)delta * (float)delta);

		physics->updatePreviousPos(currPos); 
		transform->position = nextPos;
	}
}

void PhysicsSystem::onEntityAdded(EntityID e)
{
	// Initialize previous position
	Transform* transform = ECS::GetComponent<Transform>(e);
	PhysicsBody* physics = ECS::GetComponent<PhysicsBody>(e);

	physics->updatePreviousPos(transform->position);

	// Add the entity
	entities.push_back(e);
}

void PhysicsSystem::onEntityRemoved(EntityID e)
{
	// Find and delete entity
	auto position = std::find(entities.begin(), entities.end(), e);
	entities.erase(position);
}