#include "Tilemap.hpp"
#include <nlohmann/json.hpp>

Tilemap::Tilemap(size_t tilesize) : tileset(nullptr), tilesize(tilesize) { }

void Tilemap::AddTileset(const char* path)
{
    // Load tileset texture
    tileset = TextureManager::LoadTexture(path);

    // Get width and height of tileset
    int width, height;
    SDL_QueryTexture(tileset, NULL, NULL, &width, &height);

    size_t rows = height / tilesize;
    size_t columns = width / tilesize;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // Create texture for an individual tile
            SDL_Texture* tile = SDL_CreateTexture(Engine::GetRenderer(),
                SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)tilesize, (int)tilesize);

            SDL_Rect src = { (int)(j * tilesize), (int)(i * tilesize), (int)tilesize, (int)tilesize };

            // Set the target texture to the new tile texture
            SDL_SetRenderTarget(Engine::GetRenderer(), tile);

            // Copy Current tile into the tile texture
            SDL_RenderCopy(Engine::GetRenderer(), tileset, &src, NULL);

            // Reset the render target 
            SDL_SetRenderTarget(Engine::GetRenderer(), NULL);

            // Add the new tile texture to the tiles vector
            tiles.push_back(tile);
        }
    }
}

void Tilemap::SetTilesize(size_t size) 
{
    tilesize = size;
}

void Tilemap::LoadMap(const char* path)
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
        layers.push_back(SDL_CreateTexture(Engine::GetRenderer(),
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)mapWidth, (int)mapHeight));

        // Set the texture to blend mode for transparency
        SDL_SetTextureBlendMode(layers[i], SDL_BLENDMODE_BLEND);

        // Set the target to be the map texture
        SDL_SetRenderTarget(Engine::GetRenderer(), layers[i]);

        // Clear the texture with transparent color
        SDL_SetRenderDrawColor(Engine::GetRenderer(), 0, 0, 0, 0);
        SDL_RenderClear(Engine::GetRenderer());

        // Draw each tile onto the map texture
        for (size_t j = 0; j < layout.size(); j++)
        {
            size_t row = j / cols;
            size_t col = j % cols;

            int index = layout[j] - 1;

            // Consider these as empty tiles
            if (index < 0 || index >= tiles.size())
            {
                continue;
            }

            SDL_Rect dst = { (int)(col * tilesize), (int)(row * tilesize), (int)tilesize, (int)tilesize };
            SDL_RenderCopy(Engine::GetRenderer(), tiles[index], NULL, &dst);
        }

        // Reset the render target to the default renderer target
        SDL_SetRenderTarget(Engine::GetRenderer(), NULL);
    }
}

void Tilemap::DrawMap(size_t scale)
{
	for (auto& texture : layers) 
	{
		// Draw the preloaded map texture
		if (texture)
		{
			SDL_Rect dst = { 0, 0, (int)(width * tilesize * scale), (int)(height * tilesize * scale) };
			SDL_RenderCopy(Engine::GetRenderer(), texture, NULL, &dst);
		}
	}
}