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
	static void Draw();

	// Add a new scene to the scene manager
	template <typename T>
	static void AddScene(const std::string& name) 
	{
		if (!isAdded(name)) 
		{
			auto scene = std::make_shared<T>();
			scenes[name] = std::move(scene);
			return;
		}

		fprintf(stderr, "Scene has already been added");
	}

	// Delete an existing scene
	static void DeleteScene(const std::string& name);

	// Change the current scene
	static void ChangeScene(const std::string& name);

private:
	// Map from scene type to scene itself
	static std::unordered_map<std::string, std::shared_ptr<IScene>> scenes;
	static IScene* currentScene;

	// Returns if scene has been added or not
	static bool isAdded(std::string name)
	{
		return scenes.find(name) != scenes.end();
	}

	// Private constructor to prevent instantiation
	SceneManager() = default;
};