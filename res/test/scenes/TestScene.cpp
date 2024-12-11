#include "TestScene.hpp"
#include "../../../src/input/InputMap.hpp"
#include <iostream>

void TestScene::Ready()
{
    player.Ready();

    tilemap.AddTileset("res/test/assets/untitled.png");
    tilemap.LoadMap("res/test/assets/level/untitled.json");

    tilemap.AddCollision(1, { 28, 31, 37, 81, 82, 84, 89, 90 });
}

void TestScene::Update(double delta)
{
    player.Update(delta);
}

void TestScene::Events(SDL_Event event)
{
    // Call player events
    player.Events(event);

}

void TestScene::Draw()
{
    tilemap.DrawMap();
}
