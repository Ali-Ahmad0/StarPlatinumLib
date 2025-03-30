#include "TestScene.hpp"
#include <iostream>

void TestScene::Ready()
{
    Tilemap tilemap = Tilemap(Vector2::ZERO, 16, 3);

    tilemap.AddTileset("src/test/assets/untitled.png");
    tilemap.LoadMap("src/test/assets/level/untitled.json");

    tilemap.AddCollision(1, { 80, 81, 82, 88, 89, 90, 96, 97, 98, 110, 111, 118, 119 });

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