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
	Tilemap(size_t tilesize=16);
	~Tilemap() = default;

	void SetTilesize(size_t size);

	void AddTileset(const char* path);

	void LoadMap(const char* path);
	void DrawMap(size_t scale=1);

private:
	// Tileset data
	SDL_Texture* tileset;
	std::vector<SDL_Texture*> tiles;
	size_t tilesize;

	// Height and width of tilemap
	// (In number of tiles)
	size_t width = 0;
	size_t height = 0;
	
	std::vector<SDL_Texture*> layers;
};