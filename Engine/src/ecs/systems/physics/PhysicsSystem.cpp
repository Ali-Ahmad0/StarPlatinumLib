#include "PhysicsSystem.hpp"

#include "../../ECS.hpp"
#include "../../../common/Utils.hpp"

void PhysicsSystem::update(double delta) 
{
	for (EntityID e : entities) 
	{
		PhysicsBody* body = ECS::GetComponent<PhysicsBody>(e);

		// Skip moving static bodies
		if (body->isStatic) continue;

		Transform* transform = ECS::GetComponent<Transform>(e);

		Vector2 next = Vector2::ZERO;
		Vector2 prev = body->getPreviousPos();
		Vector2 curr = transform->position;

		Vector2 acceleration = (body->force / body->getMass()) * METER;
	
		// Update instantaneous linear velocity
		body->updateLinearVelocity(curr, delta);

		// Update position using verlet integration
		next = (curr * 2) - prev + (acceleration * (float)delta * (float)delta);
		
		body->updatePreviousPos(curr); 
		transform->position = next;

		// Reset the applied force
		body->force = Vector2::ZERO;
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