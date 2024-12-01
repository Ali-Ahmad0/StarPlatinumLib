// Very basic scene changing implementation
// TO DO: Make this better

#include "InitialScene.hpp"
#include "../../res/test/scenes/TestScene.hpp"
#include "../../res/game/scenes/MainScene.hpp"

void InitialScene::Ready() 
{
	SceneManager::AddScene<TestScene>("test_scene");
	SceneManager::AddScene<MainScene>("main_scene");

	SceneManager::ChangeScene("main_scene");
}