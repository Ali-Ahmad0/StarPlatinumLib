#include "SceneManager.hpp"
#include "../ecs/ECS.hpp"
#include "../camera/Camera.hpp"

std::unordered_map<const char*, std::shared_ptr<IScene>> SceneManager::s_scenes{};
IScene* SceneManager::s_currentScene = nullptr;

void SceneManager::Update(double delta) 
{
	if (s_currentScene)
	{
		s_currentScene->Update(delta);
	}
}

void SceneManager::Events(SDL_Event event) 
{
	if (s_currentScene) 
	{
		s_currentScene->Events(event);
	}
}

void SceneManager::DeleteScene(const char* name)
{
	if (isAdded(name)) 
	{
		if (s_currentScene == s_scenes[name].get()) 
		{
			s_currentScene = nullptr;
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
		if (s_currentScene == newScene) 
		{
			return;
		}

		if (s_currentScene)
		{
			s_currentScene->Leave();
			ECS::DeleteAllEntities();
			Camera::SetOffset(Vector2::ZERO);
		}

		s_currentScene = newScene;
		s_currentScene->Ready();
		printf("[INFO]: Successfully changed scene to: %s\n", name);
	}

	else 
	{
		fprintf(stderr, "[ERROR]: Cannot change to non existent scene\n");
	}
}