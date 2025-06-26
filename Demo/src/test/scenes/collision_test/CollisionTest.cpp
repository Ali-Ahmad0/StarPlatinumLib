#include "CollisionTest.hpp"

#include "../../../src/main/debug/Debug.hpp"
#include "../../../src/ecs/ECS.hpp"
#include "../../../src/input/InputMap.hpp"

void CollisionTest::Ready()
{
	// Testing, spawn two circle collision entities
	EntityID e1 = ECS::CreateEntity();
	ECS::AddComponent<Transform>(e1, Transform(Vector2(100, 100), 20));
	ECS::AddComponent<Collider>(e1, Collider(116, 116, 32, 32));
	ECS::AddComponent<Movement>(e1, Movement(Vector2::ZERO, Vector2(100, 100)));

	EntityID e2 = ECS::CreateEntity();
	ECS::AddComponent<Transform>(e2, Transform(Vector2(150, 150), -15));
	ECS::AddComponent<Collider>(e2, Collider(170, 170, 20));

	entities.push_back(e1);
	entities.push_back(e2);
}

void CollisionTest::Update(double delta)
{
	Vector2 input;
	InputMap::GetVector("move_l", "move_r", "move_u", "move_d", input);
	ECS::GetComponent<Movement>(entities[0])->direction = input;

	for (EntityID e : entities) 
	{
		Collider* collider = ECS::GetComponent<Collider>(e);

		if (collider->getShape() == ShapeType::CIRCLE) 
		{
			Debug::DrawCircle(collider->center, collider->getRadius());
		}
		else 
		{
			Debug::DrawRect(collider->center, collider->getWidth(), collider->getHeight(), collider->rotation);
		}
	}

}
