#include "Tilemap.hpp"
#include <nlohmann/json.hpp>

Tilemap::Tilemap(const char* path, int tilesize, int rows, int columns, SDL_Renderer*  renderer)
	: tilesize(tilesize)
{
	// Load tileset texture
	tileset = TextureManager::LoadTexture(path, renderer);
	
	// Get width and height of tileset
	int width, height;
	SDL_QueryTexture(tileset, NULL, NULL, &width, &height);

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < columns; j++) 
		{
			// Create texture for an individual tile
			SDL_Texture* tile = SDL_CreateTexture(renderer, 
				SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tilesize, tilesize);
			
			SDL_Rect src = { j * tilesize, i * tilesize, tilesize, tilesize };

			// Set the target texture to the new tile texture
			SDL_SetRenderTarget(renderer, tile);

			// Copy Current tile into the tile texture
			SDL_RenderCopy(renderer, tileset, &src, NULL);

			// Reset the render target 
			SDL_SetRenderTarget(renderer, NULL);

			// Add the new tile texture to the tiles vector
			tiles.push_back(tile);
		}
	}
}

void Tilemap::LoadMap(SDL_Renderer* renderer, const char* path) 
{
	// Open file
	std::ifstream mapfile(path);

	if (!mapfile.is_open()) 
	{
		fprintf(stderr, "Failed to open map\n");
		return;
	}

	// Load file as json
	nlohmann::json mapfilejson;
	try
	{
		// Pass the file stream to parse the JSON content
		mapfilejson = nlohmann::json::parse(mapfile);
	}

	catch (const nlohmann::json::parse_error& e)
	{
		fprintf(stderr, "Unable to load tilemap: %s", e.what());
		return;
	}

	// Height and width of tilemap (in number of tiles)
	size_t rows = mapfilejson["height"];
	size_t cols = mapfilejson["width"];

	height = rows;
	width = cols;

	// Get the layout for the layer
	for (int i = 0; i < mapfilejson["layers"].size(); i++) 
	{
		auto layout = mapfilejson["layers"][i]["data"];

		// Preload the tilemap as a single texture

		// Height and width of tilemap (in number of pixels)
		size_t mapWidth = cols * tilesize;
		size_t mapHeight = rows * tilesize;

		// Create a texture to hold the entire map
		layers.push_back(SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)mapWidth, (int)mapHeight));

		// Set the target to be the map texture
		SDL_SetRenderTarget(renderer, layers[i]);

		// Draw each tile onto the map texture
		for (size_t i = 0; i < layout.size(); i++)
		{
			size_t row = i / cols;
			size_t col = i % cols;

			int index = layout[i] - 1;

			// Consider these as empty tiles
			if (index < 0 || index >= tiles.size())
			{
				continue;
			}

			SDL_Rect dst = { (int)(col * tilesize), (int)(row * tilesize), (int)tilesize, (int)tilesize };
			SDL_RenderCopy(renderer, tiles[index], NULL, &dst);
		}

		// Reset the render target to the default renderer target
		SDL_SetRenderTarget(renderer, NULL);
	}
}

void Tilemap::DrawMap(SDL_Renderer* renderer, size_t scale)
{
	for (auto& texture : layers) 
	{
		// Draw the preloaded map texture
		if (texture)
		{
			SDL_Rect dst = { 0, 0, (int)(width * tilesize * scale), (int)(height * tilesize * scale) };
			SDL_RenderCopy(renderer, texture, NULL, &dst);
		}
	}
}