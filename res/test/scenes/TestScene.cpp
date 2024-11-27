#include "TestScene.hpp"
#include "../../../src/main/Game.hpp"
#include "../../../src/input/InputMap.hpp"

void SetupKeyBindings() {
    
    InputMap::BindKey("MoveLeft", SDLK_LEFT);
    InputMap::BindKey("MoveRight", SDLK_RIGHT);
    InputMap::BindKey("MoveUp", SDLK_UP);
    InputMap::BindKey("MoveDown", SDLK_DOWN);
}

void TestScene::Ready()
{

	player.Ready();

	tilemap.AddTileset("res/test/assets/untitled.png");
	tilemap.LoadMap("res/test/assets/level/untitled.json");

	tilemap.AddCollision(1, { 28, 31, 37, 81, 82, 84, 89, 90 });
    SetupKeyBindings();
}

void TestScene::Update(double delta)
{
	//printf("Update...\n");
}

void TestScene::Events(SDL_Event event)
{
    
	player.Events(event);
    // Test key press
    if (InputMap::IsKeyJustPressed("MoveUp", event)) {
        std::cout << "Up key just pressed" << std::endl;
    }
    if (InputMap::IsKeyJustPressed("MoveLeft", event)) {
        std::cout << "MoveLeft key just pressed" << std::endl;
    }
    if (InputMap::IsKeyJustPressed("MoveRight", event)) {
        std::cout << "MoveRight key just pressed" << std::endl;
    }
    if (InputMap::IsKeyJustPressed("MoveDown", event)) {
        std::cout << "MoveDown key just pressed" << std::endl;
    }

    // Test key release
    if (InputMap::IsKeyJustReleased("Jump", event)) {
        std::cout << "Jump key released" << std::endl;
    }
    if (InputMap::IsKeyJustReleased("MoveLeft", event)) {
        std::cout << "MoveLeft key released" << std::endl;
    }
    if (InputMap::IsKeyJustReleased("MoveRight", event)) {
        std::cout << "MoveRight key released" << std::endl;
    }

    // Test key in released state
    if (InputMap::IsKeyReleased("MoveUp")) {
        std::cout << "Up key in released state" << std::endl;
    }
    if (InputMap::IsKeyReleased("MoveLeft")) {
        std::cout << "MoveLeft key in released state" << std::endl;
    }
    if (InputMap::IsKeyReleased("MoveRight")) {
        std::cout << "MoveRight key in released state" << std::endl;
    }
    if (InputMap::IsKeyReleased("MoveDown")) {
        std::cout << "MoveDown key in released state" << std::endl;
    }

    // Test key press
    if (InputMap::IsKeyPressed("MoveUp")) {
        std::cout << "Up key pressed" << std::endl;
    }
    if (InputMap::IsKeyPressed("MoveLeft")) {
        std::cout << "MoveLeft key pressed" << std::endl;
    }
    if (InputMap::IsKeyPressed("MoveRight")) {
        std::cout << "MoveRight key pressed" << std::endl;
    }
    if (InputMap::IsKeyPressed("MoveDown")) {
        std::cout << "MoveDown key pressed" << std::endl;
    }

}

void TestScene::Draw()
{
	tilemap.DrawMap();
}