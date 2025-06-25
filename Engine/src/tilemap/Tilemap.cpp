#include "Tilemap.hpp"
#include "../main/Engine.hpp"

Tilemap::Tilemap(Vector2 position, size_t tilesize, size_t scale, int8_t z_index) 
    : tileset(nullptr), position(position), tilesize(tilesize), scale(scale), z_index(z_index) { }

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
            SDL_Texture* tile = SDL_CreateTexture(StarPlatinumEngine::Renderer,
                SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)tilesize, (int)tilesize);

            SDL_Rect src = { (int)(j * tilesize), (int)(i * tilesize), (int)tilesize, (int)tilesize };

            // Set the target texture to the new tile texture
            SDL_SetRenderTarget(StarPlatinumEngine::Renderer, tile);

            // Copy Current tile into the tile texture
            SDL_RenderCopy(StarPlatinumEngine::Renderer, tileset, &src, NULL);

            // Reset the render target 
            SDL_SetRenderTarget(StarPlatinumEngine::Renderer, NULL);

            // Add the new tile texture to the tiles vector
            tiles.push_back(tile);
        }
    }
}

void Tilemap::initTextureMap(size_t layers, size_t rows, size_t cols) 
{
    printf("[INFO]: Initializing texture map...\n");
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

void Tilemap::LoadMap(const char* path)
{
    printf("[INFO]: Loading tilemap file: %s\n", path);
    // Open file
    std::ifstream mapfile(path);

    if (!mapfile.is_open())
    {
        fprintf(stderr, "[ERROR]: Failed to open tilemap\n");
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
        fprintf(stderr, "[ERROR]: Unable to load tilemap - %s\n", e.what());
        return;
    }

    // Height and width of tilemap (in number of tiles)
    height = mapfilejson["height"];
    width = mapfilejson["width"];

    orientation = mapfilejson["orientation"];

    initTextureMap(mapfilejson["layers"].size(), height, width);

    printf("[INFO]: Generating tilemap texture\n");
    // Get the layout for the layer
    for (size_t i = 0; i < mapfilejson["layers"].size(); i++)
    {
        auto layout = mapfilejson["layers"][i]["data"];

        // Preload the tilemap as a single texture
        // Height and width of tilemap (in number of pixels)
        size_t mapWidth = width * tilesize;
        size_t mapHeight = height * tilesize;

        // Create a texture to hold the entire map
        layers.push_back(SDL_CreateTexture(StarPlatinumEngine::Renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)mapWidth, (int)mapHeight));

        // Set the texture to blend mode for transparency
        SDL_SetTextureBlendMode(layers[i], SDL_BLENDMODE_BLEND);

        // Set the target to be the map texture
        SDL_SetRenderTarget(StarPlatinumEngine::Renderer, layers[i]);

        // Clear the texture with transparent color
        SDL_SetRenderDrawColor(StarPlatinumEngine::Renderer, 0, 0, 0, 0);
        SDL_RenderClear(StarPlatinumEngine::Renderer);

        // Draw each tile onto the map texture
        for (size_t j = 0; j < layout.size(); j++)
        {
            size_t row = j / width;
            size_t col = j % width;

            int index = layout[j] - 1;
            texture[i][row][col] = index;

            // Consider these as empty tiles
            if (index < 0 || index >= tiles.size())
            {
                continue;
            }

            SDL_Rect dst = { (int)(col * tilesize), (int)(row * tilesize), (int)tilesize, (int)tilesize };
            SDL_RenderCopy(StarPlatinumEngine::Renderer, tiles[index], NULL, &dst);
        }

        // Reset the render target to the default renderer target
        SDL_SetRenderTarget(StarPlatinumEngine::Renderer, NULL);
    }
    printf("[INFO]: Tilemap loaded successfully\n");

    for (auto& texture : layers) 
    {
        if (texture) 
        {
            EntityID layer = ECS::CreateEntity();
            ECS::AddComponent<Transform>(layer, Transform(position, 0.0f, scale));
            ECS::AddComponent<Sprite>(layer, Sprite(texture, 1, 1, 0, z_index));
        }
    }
}

void Tilemap::initCollisionMap() 
{
    printf("[INFO]: Initializing collision map\n");
    collision.resize(height);
    for (size_t row = 0; row < height; row++)
    {
        collision[row].resize(width, false);
    }
}

void Tilemap::generateCollisionTiles()
{
    printf("[INFO]: Generating collision tiles\n");
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

                EntityID tile = ECS::CreateEntity();
                ECS::AddComponent(tile, Transform(Vector2(x, y), 0.0, scale));
                ECS::AddComponent(tile, AABB(Vector2(w / 2, h / 2), Vector2(w, h), true, false));
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

    printf("[INFO]: Generating collision map\n");
    for (size_t row = 0; row < height; row++) 
    {
        for (size_t col = 0; col < width; col++) 
        {
            collision[row][col] = std::find(tiles.begin(), tiles.end(), texture[layer][row][col]) != tiles.end();
        }
    }

    generateCollisionTiles();
    printf("[INFO]: Collision map generated succesfully\n");
}