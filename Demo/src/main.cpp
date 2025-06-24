#include "main/Game.hpp"
#include "test/scenes/TestScene.hpp"
#include "test/scenes/StressTest.hpp"

#undef main

int main() 
{
	StarPlatinumEngine game = StarPlatinumEngine();

	SceneManager::AddScene<TestScene>("test_scene");
	SceneManager::AddScene<StressTest>("stress_test");
	SceneManager::ChangeScene("stress_test");

	game.Run();

	return 0;
}