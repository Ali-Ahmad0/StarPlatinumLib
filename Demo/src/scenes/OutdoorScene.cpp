#include "OutdoorScene.hpp"
#include "input/InputMap.hpp"
#include "tilemap/Tilemap.hpp"
#include "main/debug/Debug.hpp"

void OutdoorScene::Ready() 
{
    // Bind keys
    InputMap::BindKey("move_u", SDL_SCANCODE_UP);
    InputMap::BindKey("move_l", SDL_SCANCODE_LEFT);
    InputMap::BindKey("move_d", SDL_SCANCODE_DOWN);
    InputMap::BindKey("move_r", SDL_SCANCODE_RIGHT);

    // Initialize the world tilemap
    const size_t tileSize = 16;
    const size_t tileScaling = 3;

    Tilemap tilemap = Tilemap(tileSize, tileScaling);

    tilemap.AddTileset("src/assets/tileset/spring.png", 1);
    tilemap.AddTileset("src/assets/tileset/dirt.png", 29);
    tilemap.AddTileset("src/assets/tileset/tree.png", 44);
    tilemap.AddTileset("src/assets/tileset/water - spring.png", 76);
    tilemap.AddTileset("src/assets/tileset/texture.png", 91);
    tilemap.AddTileset("src/assets/tileset/summer.png", 123);

    tilemap.LoadTilemap("src/assets/map/untitled.json");
    tilemap.GenerateMap();

    tilemap.AddCollision(1, { 59, 60, 61, 62, 63, 64, 65, 66 });

    m_player.Ready();
}

void OutdoorScene::Update(double delta) 
{
    m_player.Update(delta);
}