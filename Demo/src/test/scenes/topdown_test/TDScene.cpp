#include "TDScene.hpp"
#include "main/debug/Debug.hpp"
#include "input/InputMap.hpp"
#include "animation/Animator.hpp"

void TDScene::Ready()
{
    // Bind keys
    InputMap::BindKey("move_u", SDL_SCANCODE_UP);
    InputMap::BindKey("move_l", SDL_SCANCODE_LEFT);
    InputMap::BindKey("move_d", SDL_SCANCODE_DOWN);
    InputMap::BindKey("move_r", SDL_SCANCODE_RIGHT);

    // Initialize the world tilemap
    const size_t tileSize = 16;
    const size_t tileScale = 3;

    Tilemap tilemap = Tilemap(tileSize, tileScale);

    tilemap.LoadTileset("src/test/assets/topdown/tileset.png");
    tilemap.LoadMapFile("src/test/assets/topdown/level/map.json");
    tilemap.GenerateMap();

    const size_t collisionLayer = 1;
    const std::vector<size_t> collisionTiles = { 89, 90, 97, 98, 110, 111, 118, 119 };

    tilemap.AddCollision(collisionLayer, collisionTiles);

    // Initialize the player
    m_player.Ready();
}

void TDScene::Update(double delta)
{
    // Update the player
    m_player.Update(delta); 
}