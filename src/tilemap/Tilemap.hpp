#include <SDL.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include "..\texture\TextureManager.hpp"

class Tilemap 
{
public:

	Tilemap() = default;

	Tilemap(const char* path, int tilesize, int rows, int columns, SDL_Renderer* renderer);
	~Tilemap() = default;

	void LoadMap(SDL_Renderer* renderer, const char* path);
	void DrawMap(SDL_Renderer* renderer, size_t scale);

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