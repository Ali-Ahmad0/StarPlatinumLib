#pragma once

#include "Scene.hpp"

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <iostream>

class SceneManager 
{
public:
	static void Update(double delta);
	static void Events(SDL_Event event);

	// Add a new scene to the scene manager
	template <typename T>
	static void AddScene(const char* name) 
	{
		if (!isAdded(name)) 
		{
			auto scene = std::make_shared<T>();
			s_scenes[name] = std::move(scene);
			return;
		}

		fprintf(stderr, "[ERROR]: Scene has already been added\n");
	}

	// Delete an existing scene
	static void DeleteScene(const char* name);

	// Change the current scene
	static void ChangeScene(const char* name);

private:
	// Map from scene type to scene itself
	static std::unordered_map<const char*, std::shared_ptr<IScene>> s_scenes;
	static IScene* s_currentScenes;

	// Returns if scene has been added or not
	static bool isAdded(const char* name)
	{
		return s_scenes.find(name) != s_scenes.end();
	}

	// Private constructor to prevent instantiation
	SceneManager() = default;
};