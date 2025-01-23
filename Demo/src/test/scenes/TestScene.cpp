#include "TestScene.hpp"
#include <iostream>

void TestScene::Ready()
{
    Tilemap tilemap = Tilemap(Vector2(0, 0), 16, 4);

    tilemap.AddTileset("src/test/assets/untitled.png");
    tilemap.LoadMap("src/test/assets/level/untitled.json");

    tilemap.AddCollision(1, { 28, 31, 37, 81, 82, 84, 89, 90 });

    player.Ready();
}

void TestScene::Update(double delta)
{
    player.Update(delta);
}

void TestScene::Events(SDL_Event event) 
{
    player.Events(event);
}