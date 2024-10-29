#include "Tilemap.hpp"
#include "../main/Game.hpp"

Tilemap::Tilemap(const size_t tilesize, const size_t scale) : tileset(nullptr), tilesize(tilesize), scale(scale) { }

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

void Tilemap::SetTilesize(const size_t size) 
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

    initTextureMap(mapfilejson["layers"].size(), height, width);

    // Get the layout for the layer
    for (size_t i = 0; i < mapfilejson["layers"].size(); i++)
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
            texture[i][row][col] = index;

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

void Tilemap::DrawMap()
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

void Tilemap::generateCollisionTiles()
{
    // Loop through each row to identify contiguous blocks of collidable tiles
    for (size_t row = 0; row < height; row++)
    {
        size_t col = 0;

        while (col < width)
        {
            // Start of a collidable block
            if (collision[row][col])
            {
                size_t startCol = col;

                // End of the collidable block horizontally
                while (col < width && collision[row][col])
                {
                    col++;
                }

                size_t endCol = col;

                // Extend the block vertically
                size_t endRow = row;
                bool isCollidable = true;

                while (isCollidable && endRow + 1 < height)
                {
                    // Check if the next row has a collidable block
                    for (size_t currentCol = startCol; currentCol < endCol; currentCol++)
                    {
                        if (!collision[endRow + 1][currentCol])
                        {
                            isCollidable = false;
                            break;
                        }
                    }

                    if (isCollidable)
                    {
                        endRow++;
                    }
                }

                
                float w = (float)(endCol - startCol) * tilesize;
                float h = (float)(endRow - row + 1) * tilesize;

                float x = (float)startCol * tilesize * scale;
                float y = (float)row * tilesize * scale;

                std::cout << "X: " << x << " Y: " << y << " W: " << w << " H: " << h << '\n';

                EntityID tile = Engine::GetECS().CreateEntity();
                Engine::GetECS().AddComponent(tile, Transform(Vector2(x, y), scale));
                Engine::GetECS().AddComponent(tile, AABB(Vector2(w / 2, h / 2), Vector2(w, h), true, false));
            }
            else
            {
                col++;
            }
        }
    }
}


// Set collision true for a certain number of tiles
void Tilemap::AddCollision(size_t layer, const std::vector<size_t>& tiles) 
{
    initCollisionMap();

    for (size_t row = 0; row < height; row++) 
    {
        for (size_t col = 0; col < width; col++) 
        {
            collision[row][col] = std::find(tiles.begin(), tiles.end(), texture[layer][row][col]) != tiles.end();
            std::cout << collision[row][col] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    generateCollisionTiles();
}