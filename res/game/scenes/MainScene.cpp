#include "MainScene.hpp"
#include "../../../src/main/Game.hpp"

void MainScene::Ready() 
{
	tilemap.AddTileset("res/game/assets/tilemap/tileset.png");
	tilemap.LoadMap("res/game/assets/level/debug.json");

	tilemap.AddCollision(0, { 110, 111, 112, 130, 131, 132, 175, 176, 177, 270, 271, 272, 290, 291, 292 });

	player.Ready();
}

void MainScene::Update(double delta) 
{
	
}

void MainScene::Events(SDL_Event event) 
{
	player.Events(event);
}

void MainScene::Draw() 
{
	tilemap.DrawMap();
}