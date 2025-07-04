#include "PhysicsTest.hpp"
#include "ecs/ECS.hpp"
#include "main/debug/Debug.hpp"

// Spawn a ball and test to see it fall under gravity
void PhysicsTest::Ready()
{
	ball = ECS::CreateEntity();
	ECS::AddComponent<Transform>(ball, Transform(Vector2(310, 60)));
	ECS::AddComponent<PhysicsBody>(ball, PhysicsBody(1));
	ECS::AddComponent<Collider>(ball, Collider(10, 10, 20));
}

void PhysicsTest::Update(double delta)
{
	Transform* transform = ECS::GetComponent<Transform>(ball);
	PhysicsBody* physics = ECS::GetComponent<PhysicsBody>(ball);
	Collider* collider = ECS::GetComponent<Collider>(ball);

	Debug::DrawCircle(collider->centerOffset + transform->position, collider->getRadius());
	physics->applyForce(Vector2(0, 9.81f));
}
