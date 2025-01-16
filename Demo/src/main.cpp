#include "main/Game.hpp"
#include "test/scenes/TestScene.hpp"

#undef main

Engine* game = nullptr;

int main() 
{
	game = new Engine();

	game->Init();

	// Change to initial scene
	SceneManager::AddScene<TestScene>("test_scene");
	SceneManager::ChangeScene("test_scene");

	game->GameLoop();
	game->Exit();

	// Free memory
	delete game;
	game = nullptr;

	return 0;
}