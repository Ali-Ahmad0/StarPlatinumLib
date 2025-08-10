#include "TDScene.hpp"
#include "main/debug/Debug.hpp"
#include <iostream>

void TDScene::Ready()
{
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