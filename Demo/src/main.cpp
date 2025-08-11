#include "main/Engine.hpp"

#include "test/scenes/topdown_test/TDScene.hpp"
#include "test/scenes/stress_test/StressTest.hpp"
#include "test/scenes/physics_test/PhysicsTest.hpp"

#undef main

int main() 
{
	StarPlatinumEngine game = StarPlatinumEngine();

	SceneManager::AddScene<TDScene>("topdown_test");
	SceneManager::AddScene<StressTest>("stress_test");
	SceneManager::AddScene<PhysicsTest>("physics_test");

	SceneManager::ChangeScene("topdown_test");

	game.Run();

	return 0;
}