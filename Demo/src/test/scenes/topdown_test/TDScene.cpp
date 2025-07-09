#include "TDScene.hpp"
#include <iostream>

void TDScene::Ready()
{
    Tilemap tilemap = Tilemap(Vector2::ZERO, 16, 3);

    tilemap.AddTileset("src/test/assets/topdown/tileset.png");
    tilemap.LoadMap("src/test/assets/topdown/level/map.json");

    tilemap.AddCollision(1, { 89, 90, 97, 98, 110, 111, 118, 119 });

    player.Ready();
}

void TDScene::Update(double delta)
{
    player.Update(delta);
}

void TDScene::Events(SDL_Event event) 
{
    player.Events(event);
}