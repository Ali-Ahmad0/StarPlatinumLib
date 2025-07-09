#include "CollisionTest.hpp"

#include "main/debug/Debug.hpp"
#include "ecs/ECS.hpp"
#include "input/InputMap.hpp"
#include "viewport/ViewPort.hpp"

void CollisionTest::Ready()
{
	// Testing, spawn two circle collision entities
	EntityID e1 = ECS::CreateEntity();
	ECS::AddComponent<Transform>(e1, Transform(Vector2(64, 128)));
	ECS::AddComponent<PhysicsBody>(e1, PhysicsBody(1));
	ECS::AddComponent<Collider>(e1, Collider(0, 0, 32));

	EntityID e2 = ECS::CreateEntity();
	ECS::AddComponent<Transform>(e2, Transform(Vector2(256, 128)));
	ECS::AddComponent<PhysicsBody>(e2, PhysicsBody(1));
	ECS::AddComponent<Collider>(e2, Collider(0, 0, 48, 48));

	entities.push_back(e1);
	entities.push_back(e2);
}

void CollisionTest::Update(double delta)
{
	PhysicsBody* body = ECS::GetComponent<PhysicsBody>(0);
	Vector2 input;
	InputMap::GetVector("move_l", "move_r", "move_u", "move_d", input);

	body->force = input * 10;

	for (EntityID e : entities) 
	{
		Transform* transform = ECS::GetComponent<Transform>(e);
		Collider* collider = ECS::GetComponent<Collider>(e);

		if (collider->getShape() == ShapeType::BOX) 
		{
			Debug::DrawRect(
				transform->position + collider->centerOffset * (float)transform->scale,
				collider->getWidth() * (float)transform->scale,
				collider->getHeight() * (float)transform->scale
			);
		}
		else 
		{
			Debug::DrawCircle(
				transform->position + collider->centerOffset * (float)transform->scale,
				collider->getRadius() * (float)transform->scale
			);
		}
	}
}
