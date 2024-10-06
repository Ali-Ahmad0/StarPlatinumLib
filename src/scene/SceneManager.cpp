#include "SceneManager.hpp"

std::unordered_map<std::string, std::shared_ptr<IScene>> SceneManager::scenes{};
IScene* SceneManager::currentScene = nullptr;

void SceneManager::Update(double delta) 
{
	if (currentScene)
	{
		currentScene->Update(delta);
	}
}

void SceneManager::Draw() 
{
	if (currentScene) 
	{
		currentScene->Draw();
	}
}

void SceneManager::DeleteScene(const std::string& name) 
{
	if (isAdded(name)) 
	{
		if (currentScene == scenes[name].get()) 
		{
			currentScene = nullptr;
		}
		scenes.erase(name);
	}
	else 
	{
		fprintf(stderr, "Cannot delete a scene that does not exist");
	}
}

void SceneManager::ChangeScene(const std::string& name) 
{
	if (isAdded(name)) 
	{
		auto newScene = scenes[name].get();
		if (currentScene != newScene) 
		{
			currentScene = newScene;
			currentScene->Ready();
		}
	}

	else 
	{
		fprintf(stderr, "Cannot change scene as scene does not exist");
	}
}