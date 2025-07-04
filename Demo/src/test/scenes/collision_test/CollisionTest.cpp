#include "CollisionTest.hpp"

#include "main/debug/Debug.hpp"
#include "ecs/ECS.hpp"
#include "input/InputMap.hpp"
#include "viewport/ViewPort.hpp"

void CollisionTest::Ready()
{
	// Testing, spawn two circle collision entities
	EntityID e1 = ECS::CreateEntity();
	ECS::AddComponent<Transform>(e1, Transform(Vector2(64, 64), 0, 2));
	ECS::AddComponent<Collider>(e1, Collider(16, 16, 32, 32));
	ECS::AddComponent<Movement>(e1, Movement(Vector2::ZERO, Vector2(100, 100)));

	EntityID e2 = ECS::CreateEntity();
	ECS::AddComponent<Transform>(e2, Transform(Vector2(128, 128)));
	ECS::AddComponent<Collider>(e2, Collider(20, 20, 20));

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
		Transform* transform = ECS::GetComponent<Transform>(e);
		Collider* collider = ECS::GetComponent<Collider>(e);


		// Draw collider shapes
		if (collider->getShape() == ShapeType::CIRCLE) 
		{
			Debug::DrawCircle(
				collider->centerOffset + transform->position, collider->getRadius()
			);
		}
		else 
		{
			transform->rotation++;

			Debug::DrawRect(
				collider->centerOffset + transform->position, 
				collider->getWidth() * transform->scale, 
				collider->getHeight() * transform->scale, transform->rotation
			);

			std::array<Vector2, 4> vertices = collider->getTransformedVertices(transform);
		}

		// Print if collision occurs
		if (e == 0 && collider->isColliding)
		{
			std::cout << "Collision!\n";
		}
	}

}
