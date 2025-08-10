#pragma once

#include <SDL.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <json.hpp>
#include <iostream>

#include "..\texture\TextureManager.hpp"

class Tilemap 
{
public:
	Tilemap() = default;
	Tilemap(size_t tilesize=16, size_t scale = 1);

	void LoadTileset(const char* path);
	void LoadMapFile(const char* path);
	void GenerateMap(
		const Vector2& origin = Vector2::ZERO, 
		float rotation = 0.0f, 
		int8_t z_index = 0
	);
	void AddCollision(size_t layer, const std::vector<size_t>& tiles);

private:	
	// Height and width of tilemap
	size_t m_mapWidth = 0;
	size_t m_mapHeight = 0;

	// Size of each tile
	size_t m_tilesize;
	size_t m_tileScale;

	// Array of tilemap layers
	std::vector<SDL_Texture*> m_layers;

	// Array of tile textures
	std::vector<SDL_Texture*> m_tiles;

	// Texture map, stores index of textures in tilemap
	std::vector<std::vector<std::vector<int>>> m_texture;

	// Collision map, stores if a tile has collision or not
	std::vector<std::vector<bool>> m_collision;

	void initTextureMap(size_t layers, size_t rows, size_t cols);
	void initCollisionMap();

	void generateCollisionTiles();
};