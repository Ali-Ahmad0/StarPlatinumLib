#pragma once

#include <SDL.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <iostream>

#include "..\texture\TextureManager.hpp"
#include "..\main\Game.hpp"

class Tilemap 
{
public:
	Tilemap(const size_t tilesize=16, const size_t scale=1);
	~Tilemap() = default;

	void SetTilesize(const size_t size);

	void AddTileset(const char* path);

	void LoadMap(const char* path);
	void DrawMap();

	void AddCollision(size_t layer, const std::vector<size_t>& tiles);

private:
	// Tileset data
	SDL_Texture* tileset;
	std::vector<SDL_Texture*> tiles;
	size_t tilesize;

	// Height and width of tilemap
	// (In number of tiles)
	size_t width = 0;
	size_t height = 0;

	size_t scale;
	
	// Tilemap layers
	std::vector<SDL_Texture*> layers;

	// Texture Map: Stores index of textures in tilemap
	std::vector<std::vector<std::vector<int>>> texture;

	// Collision Map: Stores if a tile has collision or not
	std::vector<std::vector<bool>> collision;

	// Initialize texture map
	void initTextureMap(size_t layers, size_t rows, size_t cols)
	{
		texture.resize(layers);
		for (size_t layer = 0; layer < layers; layer++) 
		{
			texture[layer].resize(rows);
			for (size_t row = 0; row < rows; row++)
			{
				texture[layer][row].resize(cols, -1);
			}
		}
	}

	// Initialize collision map
	void initCollisionMap(size_t rows, size_t cols) 
	{
		collision.resize(rows);
		for (size_t row = 0; row < rows; row++) 
		{
			collision[row].resize(cols, false);
		}
	}
};