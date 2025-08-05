#include "PhysicsTest.hpp"
#include "ecs/ECS.hpp"
#include "input/InputMap.hpp"
#include "main/debug/Debug.hpp"
#include "text/TextManager.hpp"
#include <string>
#include <sstream>

#define MARGIN 4.0f

void PhysicsTest::Ready()
{
    TextManager::LoadFont("default", "src/test/assets/font.ttf");
}

void PhysicsTest::Update(double delta)
{
    Vector2 ringCenter(320, 240); // Center of screen / ring
    float ringRadius = 200.0f;    // Ring radius in pixels

    for (EntityID e = 0; e < ECS::GetEntityCount(); e++)
    {
        auto* transform = ECS::GetComponent<Transform>(e);
        auto* collider = ECS::GetComponent<Collider>(e);
        auto* verlet = ECS::GetComponent<VerletObject>(e);

        if (collider && collider->getShape() == ShapeType::CIRCLE)
        {
            Debug::DrawCircle(transform->position + collider->centerOffset, collider->getRadius());

            if (verlet && !verlet->isStationary)
            {
                // Apply gravity
                verlet->applyForce(Vector2(0, 9.81f));

                // Ring constraint - keep balls inside the ring
                Vector2 ballPosition = transform->position;
                Vector2 fromCenter = ballPosition - ringCenter;
                float distanceFromCenter = fromCenter.magnitude();

                // If ball is outside the ring, push it back in
                if (distanceFromCenter > ringRadius - collider->getRadius())
                {
                    // Normalize the direction vector
                    Vector2 direction = fromCenter.normalize();

                    // Calculate how far outside the ring the ball is
                    float overshoot = distanceFromCenter - (ringRadius - collider->getRadius());

                    // Push the ball back inside
                    transform->position = ballPosition - (direction * overshoot);

                    // Add bounce off of walls
                    Vector2 velocity = transform->position - verlet->prevPosition;
                    Vector2 reflectedVelocity = velocity - (direction * (2.0f * Vector2::dot(velocity, direction)));
                    verlet->prevPosition = transform->position - (reflectedVelocity * 0.8f);
                }
            }
        }
    }

    // Draw the ring boundary for visualization
    Debug::DrawCircle(ringCenter, ringRadius + MARGIN, Color::WHITE, false);

    // Update FPS only once per second
    static double fpsTimer = 0.0;
    static int cachedFPS = 0;
    static std::string cachedFPSText = "FPS: 0";

    fpsTimer += delta;

    // Update every 0.5 seconds
    if (fpsTimer >= 0.5)
    {
        // Calculate FPS
        double targetDelta = 1000 / 60;
        cachedFPS = (int)(targetDelta / (delta * 1000) * 60);

        // Format FPS string
        std::ostringstream fpsStream;
        fpsStream << "FPS: " << cachedFPS;
        cachedFPSText = fpsStream.str();

        fpsTimer = 0.0;
    }

    // Draw the FPS text
    TextManager::DrawText(cachedFPSText.c_str(), "default", Vector2(16, 16));

    // Get entity count
    std::ostringstream countStream;
    countStream << "Entities: " << ECS::GetEntityCount();
    std::string countText = countStream.str();

    // Draw entity count
    TextManager::DrawText(countText.c_str(), "default", Vector2(500, 16));
}

void PhysicsTest::Events(SDL_Event event)
{
    Vector2 mousePosition;
    InputMap::GetMousePositionWindow(mousePosition);

    if (InputMap::MouseLeftPressed(event))
    {
        // Ring parameters
        Vector2 ringCenter(320, 240);

        // Calculate initial force direction (from center to mouse)
        Vector2 forceDirection = mousePosition - ringCenter;
        float forceMagnitude = forceDirection.magnitude() * 1.25f;
        forceDirection = forceDirection.normalize();

        // Create ball at ring center
        EntityID ball = ECS::CreateEntity();
        ECS::AddComponent<Transform>(ball, Transform(ringCenter));
        ECS::AddComponent<Collider>(ball, Collider(Vector2(4, 4), 8.0f));

        // Create verlet object with initial velocity
        auto verletObj = VerletObject(ringCenter);

        float initialDeltaTime = 1.0f / 60.0f;
        Vector2 initialVelocity = forceDirection * forceMagnitude;
        verletObj.prevPosition = ringCenter - (initialVelocity * initialDeltaTime);

        ECS::AddComponent<VerletObject>(ball, verletObj);
    }
}