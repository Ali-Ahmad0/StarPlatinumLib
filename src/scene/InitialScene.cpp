#include "InitialScene.hpp"
#include "../../res/scenes/TestScene.hpp"

void InitialScene::Ready() 
{
	SceneManager::AddScene<TestScene>("test_scene");
	SceneManager::ChangeScene("test_scene");
}

void InitialScene::Update(double delta) 
{

}

void InitialScene::Draw() 
{

}