#include "main/Engine.hpp"

#include "test/scenes/world_test/TestScene.hpp"
#include "test/scenes/stress_test/StressTest.hpp"
#include "test/scenes/collision_test/CollisionTest.hpp"

#undef main

int main() 
{
	StarPlatinumEngine game = StarPlatinumEngine();

	SceneManager::AddScene<TestScene>("test_scene");
	SceneManager::AddScene<StressTest>("stress_test");
	SceneManager::AddScene<CollisionTest>("collision_test");

	SceneManager::ChangeScene("collision_test");

	game.Run();

	return 0;
}