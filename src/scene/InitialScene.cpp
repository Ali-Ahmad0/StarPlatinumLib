// Very basic scene changing implementation
// TO DO: Make this better

#include "InitialScene.hpp"
#include "../../res/test/scenes/TestScene.hpp"

void InitialScene::Ready() 
{
	SceneManager::AddScene<TestScene>("test_scene");
	SceneManager::ChangeScene("test_scene");
}