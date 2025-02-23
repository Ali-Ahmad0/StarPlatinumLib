#include "main/Game.hpp"
#include "test/scenes/TestScene.hpp"

#undef main

int main() 
{
	StarPlatinumEngine game = StarPlatinumEngine();

	SceneManager::AddScene<TestScene>("test_scene");
	SceneManager::ChangeScene("test_scene");

	game.Run();

	return 0;
}