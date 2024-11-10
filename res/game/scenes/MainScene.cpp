#include "MainScene.hpp"
#include "../../../src/main/Game.hpp"

void MainScene::Ready() 
{
	//playerTexture = TextureManager::LoadTexture("res/game/assets/player/player.png");
	//player = Engine::GetECS().CreateEntity();

	tilemap.AddTileset("res/game/assets/tilemap/tileset.png");
	tilemap.LoadMap("res/game/assets/level/debug.json");
}

void MainScene::Update(double delta) 
{
	
}

void MainScene::Events(SDL_Event event) 
{

}

void MainScene::Draw() 
{
	tilemap.DrawMap();
}