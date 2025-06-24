#include "SceneManager.hpp"

std::unordered_map<const char*, std::shared_ptr<IScene>> SceneManager::scenes{};
IScene* SceneManager::currentScene = nullptr;

void SceneManager::Update(double delta) 
{
	if (currentScene)
	{
		currentScene->Update(delta);
	}
}

void SceneManager::Events(SDL_Event event) 
{
	if (currentScene) 
	{
		currentScene->Events(event);
	}
}

void SceneManager::DeleteScene(const char* name)
{
	if (isAdded(name)) 
	{
		if (currentScene == scenes[name].get()) 
		{
			currentScene = nullptr;
		}
		scenes.erase(name);
		printf("[INFO]: Successfully deleted scene: %s\n", name);
	}
	else 
	{
		fprintf(stderr, "[ERROR]: Cannot delete a scene that does not exist\n");
	}
}

void SceneManager::ChangeScene(const char* name) 
{
	if (isAdded(name)) 
	{
		auto newScene = scenes[name].get();
		if (currentScene != newScene) 
		{
			currentScene = newScene;
			currentScene->Ready();
			printf("[INFO]: Successfully changed scene to: %s\n", name);
		}
	}

	else 
	{
		fprintf(stderr, "[ERROR]: Cannot change to non existent scene\n");
	}
}