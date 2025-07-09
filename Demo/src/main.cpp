#include "main/Engine.hpp"

#include "test/scenes/world_test/TestScene.hpp"
#include "test/scenes/stress_test/StressTest.hpp"
#include "test/scenes/collision_test/CollisionTest.hpp"
#include "test/scenes/physics_test/PhysicsTest.hpp"

#include "input/InputMap.hpp"

#undef main

int main() 
{
	StarPlatinumEngine game = StarPlatinumEngine();

	SceneManager::AddScene<TestScene>("test_scene");
	SceneManager::AddScene<StressTest>("stress_test");
	SceneManager::AddScene<CollisionTest>("collision_test");
	SceneManager::AddScene<PhysicsTest>("physics_test");

	// Bind keys
	InputMap::BindKey("move_u", SDL_SCANCODE_UP);
	InputMap::BindKey("move_l", SDL_SCANCODE_LEFT);
	InputMap::BindKey("move_d", SDL_SCANCODE_DOWN);
	InputMap::BindKey("move_r", SDL_SCANCODE_RIGHT);

	SceneManager::ChangeScene("test_scene");

	game.Run();

	return 0;
}