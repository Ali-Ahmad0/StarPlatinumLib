#pragma once

#include <SDL.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <iostream>

#include "..\texture\TextureManager.hpp"

class Tilemap 
{
public:
	Tilemap() = default;
	Tilemap(Vector2 position=Vector2(0, 0), size_t tilesize=16, size_t scale=1, int8_t z_index=0);
	~Tilemap() = default;

	void AddTileset(const char* path);

	void LoadMap(const char* path);
	void DrawMap();

	void AddCollision(size_t layer, const std::vector<size_t>& tiles);

private:
	Vector2 position;
	
	// Tileset data
	SDL_Texture* tileset;
	std::vector<SDL_Texture*> tiles;
	size_t tilesize;

	// Height and width of tilemap
	// (In number of tiles)
	size_t width = 0;
	size_t height = 0;

	size_t scale;
	int8_t z_index;
	
	// Tilemap layers
	std::vector<SDL_Texture*> layers;

	// Texture Map: Stores index of textures in tilemap
	std::vector<std::vector<std::vector<int>>> texture;

	// Collision Map: Stores if a tile has collision or not
	std::vector<std::vector<bool>> collision;
	std::vector<Rectangle> rectangles;

	// Initialize texture map
	void initTextureMap(size_t layers, size_t rows, size_t cols);

	// Initialize collision map
	void initCollisionMap();

	// Generate collision entities
	void generateCollisionTiles();
};