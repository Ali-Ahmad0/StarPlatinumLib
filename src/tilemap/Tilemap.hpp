#pragma once

#include <SDL.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include "..\texture\TextureManager.hpp"
#include "..\main\Game.hpp"

class Tilemap 
{
public:

	Tilemap() = default;

	Tilemap(const char* path, size_t tilesize=16);
	~Tilemap() = default;

	void LoadMap(const char* path);
	void DrawMap(size_t scale=1);

private:
	SDL_Texture* tileset;
	std::vector<SDL_Texture*> tiles;
	size_t tilesize;

	// Height and width of tilemap
	// (In number of tiles)
	size_t width = 0;
	size_t height = 0;
	
	std::vector<SDL_Texture*> layers;
};