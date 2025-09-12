#include "main/Engine.hpp"
#include "scenes/OutdoorScene.hpp"

#undef main

int main() 
{
	StarPlatinumEngine game = StarPlatinumEngine();

	SceneManager::AddScene<OutdoorScene>("outdoor_scene");
	SceneManager::ChangeScene("outdoor_scene");

	game.Run();

	return 0;
}