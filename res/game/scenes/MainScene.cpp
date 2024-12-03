#include "MainScene.hpp"
#include "../../../src/main/Game.hpp"
#include "../../../src/input/InputMap.hpp"
#include <iostream>

void MainScene::Ready()
{
    tilemap.AddTileset("res/game/assets/tilemap/tileset.png");
    tilemap.LoadMap("res/game/assets/level/debug.json");

    tilemap.AddCollision(0, { 110, 111, 112, 130, 131, 132, 175, 176, 177, 270, 271, 272, 290, 291, 292 });

    player.Ready();

    // Bind keys for testing
    InputMap::BindKey("Jump", SDL_SCANCODE_SPACE);
    InputMap::BindKey("Left", SDL_SCANCODE_A);
    InputMap::BindKey("Right", SDL_SCANCODE_D);
    InputMap::BindKey("Up", SDL_SCANCODE_W);
    InputMap::BindKey("Down", SDL_SCANCODE_S);
}

void MainScene::Update(double delta)
{
    player.Update(delta);
}

void MainScene::Events(SDL_Event event)
{
    // Call player events
    player.Events(event);

    // Debug print to ensure Events function is called
    //std::cout << "Event received: " << event.type << std::endl;

    // Handle keyboard events
    if (InputMap::IsKeyPressed("Jump"))
    {
        std::cout << "Jump key pressed" << std::endl;
    }

    if (InputMap::IsKeyPressed("Left"))
    {
        std::cout << "Left key pressed" << std::endl;
    }

    if (InputMap::IsKeyPressed("Right"))
    {
        std::cout << "Right key pressed" << std::endl;
    }

    if (InputMap::IsKeyPressed("Up"))
    {
        std::cout << "Up key pressed" << std::endl;
    }

    if (InputMap::IsKeyPressed("Down"))
    {
        std::cout << "Down key pressed" << std::endl;
    }


    // Handle mouse events
    if (InputMap::MouseLeftPressed(event))
    {
        std::cout << "Mouse left button pressed" << std::endl;
    }
    if (InputMap::MouseMiddlePressed(event))
    {
        std::cout << "Mouse middle button pressed" << std::endl;
    }
    if (InputMap::MouseRightPressed(event))
    {
        std::cout << "Mouse right button pressed" << std::endl;
    }
}

void MainScene::Draw()
{
    tilemap.DrawMap();
}
