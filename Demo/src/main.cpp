#include "main/Engine.hpp"

#include "test/scenes/topdown_test/TDScene.hpp"
#include "test/scenes/stress_test/StressTest.hpp"

#include "input/InputMap.hpp"

#undef main

int main() 
{
	StarPlatinumEngine game = StarPlatinumEngine();

	SceneManager::AddScene<TDScene>("topdown_test");
	SceneManager::AddScene<StressTest>("stress_test");

	// Bind keys
	InputMap::BindKey("move_u", SDL_SCANCODE_UP);
	InputMap::BindKey("move_l", SDL_SCANCODE_LEFT);
	InputMap::BindKey("move_d", SDL_SCANCODE_DOWN);
	InputMap::BindKey("move_r", SDL_SCANCODE_RIGHT);
	InputMap::BindKey("spawn", SDL_SCANCODE_RETURN);

	SceneManager::ChangeScene("topdown_test");

	game.Run();

	return 0;
}