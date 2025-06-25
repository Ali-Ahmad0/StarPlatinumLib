#include "main/Engine.hpp"
#include "test/scenes/TestScene.hpp"
#include "test/scenes/StressTest.hpp"

#undef main

int main() 
{
	StarPlatinumEngine game = StarPlatinumEngine();

	SceneManager::AddScene<TestScene>("test_scene");
	SceneManager::AddScene<StressTest>("stress_test");
	SceneManager::ChangeScene("test_scene");

	game.Run();

	return 0;
}