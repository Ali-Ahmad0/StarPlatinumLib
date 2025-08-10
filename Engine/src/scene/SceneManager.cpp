#include "SceneManager.hpp"

std::unordered_map<const char*, std::shared_ptr<IScene>> SceneManager::s_scenes{};
IScene* SceneManager::s_currentScenes = nullptr;

void SceneManager::Update(double delta) 
{
	if (s_currentScenes)
	{
		s_currentScenes->Update(delta);
	}
}

void SceneManager::Events(SDL_Event event) 
{
	if (s_currentScenes) 
	{
		s_currentScenes->Events(event);
	}
}

void SceneManager::DeleteScene(const char* name)
{
	if (isAdded(name)) 
	{
		if (s_currentScenes == s_scenes[name].get()) 
		{
			s_currentScenes = nullptr;
		}
		s_scenes.erase(name);
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
		auto newScene = s_scenes[name].get();
		if (s_currentScenes != newScene) 
		{
			if (s_currentScenes) s_currentScenes->Leave();

			s_currentScenes = newScene;
			s_currentScenes->Ready();
			printf("[INFO]: Successfully changed scene to: %s\n", name);
		}
	}

	else 
	{
		fprintf(stderr, "[ERROR]: Cannot change to non existent scene\n");
	}
}