#include "Tilemap.hpp"
#include "../main/Engine.hpp"

Tilemap::Tilemap(size_t tilesize, size_t scale) : m_tilesize(tilesize), m_tileScale(scale) { }

void Tilemap::LoadTileset(const char* path)
{
    // Load tileset texture
    SDL_Texture* tileset = TextureManager::LoadTexture(path);

    // Get width and height of tileset
    int width, height;
    SDL_QueryTexture(tileset, NULL, NULL, &width, &height);

    size_t rows = height / m_tilesize;
    size_t columns = width / m_tilesize;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // Create texture for an individual tile
            SDL_Texture* tile = SDL_CreateTexture(ViewPort::GetRenderer(),
                SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)m_tilesize, (int)m_tilesize);

            SDL_Rect src = { (int)(j * m_tilesize), (int)(i * m_tilesize), (int)m_tilesize, (int)m_tilesize };

            // Set the target texture to the new tile texture
            SDL_SetRenderTarget(ViewPort::GetRenderer(), tile);

            // Copy Current tile into the tile texture
            SDL_RenderCopy(ViewPort::GetRenderer(), tileset, &src, NULL);

            // Reset the render target 
            SDL_SetRenderTarget(ViewPort::GetRenderer(), NULL);

            // Add the new tile texture to the tiles vector
            m_tiles.push_back(tile);
        }
    }
}

void Tilemap::initTextureMap(size_t layers, size_t rows, size_t cols) 
{
    printf("[INFO]: Initializing texture map...\n");
    m_texture.resize(layers);
    for (size_t layer = 0; layer < layers; layer++)
    {
        m_texture[layer].resize(rows);
        for (size_t row = 0; row < rows; row++)
        {
            m_texture[layer][row].resize(cols, -1);
        }
    }
}

void Tilemap::LoadMapFile(const char* path)
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
    m_mapHeight = mapfilejson["height"];
    m_mapWidth = mapfilejson["width"];

    initTextureMap(mapfilejson["layers"].size(), m_mapHeight, m_mapWidth);

    printf("[INFO]: Generating tilemap texture\n");
    // Get the layout for the layer
    for (size_t i = 0; i < mapfilejson["layers"].size(); i++)
    {
        auto layout = mapfilejson["layers"][i]["data"];

        // Preload the tilemap as a single texture
        // Height and width of tilemap (in number of pixels)
        size_t mapWidth = m_mapWidth * m_tilesize;
        size_t mapHeight = m_mapHeight * m_tilesize;

        // Create a texture to hold the entire map
        m_layers.push_back(SDL_CreateTexture(ViewPort::GetRenderer(),
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)mapWidth, (int)mapHeight));

        // Set the texture to blend mode for transparency
        SDL_SetTextureBlendMode(m_layers[i], SDL_BLENDMODE_BLEND);

        // Set the target to be the map texture
        SDL_SetRenderTarget(ViewPort::GetRenderer(), m_layers[i]);

        // Clear the texture with transparent color
        SDL_SetRenderDrawColor(ViewPort::GetRenderer(), 0, 0, 0, 0);
        SDL_RenderClear(ViewPort::GetRenderer());

        // Draw each tile onto the map texture
        for (size_t j = 0; j < layout.size(); j++)
        {
            size_t row = j / m_mapWidth;
            size_t col = j % m_mapWidth;

            int index = layout[j] - 1;
            m_texture[i][row][col] = index;

            // Consider these as empty tiles
            if (index < 0 || index >= m_tiles.size())
            {
                continue;
            }

            SDL_Rect dst = { (int)(col * m_tilesize), (int)(row * m_tilesize), (int)m_tilesize, (int)m_tilesize };
            SDL_RenderCopy(ViewPort::GetRenderer(), m_tiles[index], NULL, &dst);
        }

        // Reset the render target to the default renderer target
        SDL_SetRenderTarget(ViewPort::GetRenderer(), NULL);
    }
    printf("[INFO]: Tilemap file loaded successfully\n");
}

void Tilemap::GenerateMap(const Vector2& origin, float rotation, int8_t z_index)
{
    for (auto& layerTexture : m_layers)
    {
        if (layerTexture)
        {
            EntityID layerEntity = ECS::CreateEntity();
            ECS::AddComponent<Transform>(layerEntity, Transform(origin, rotation, m_tileScale));
            ECS::AddComponent<Sprite>(layerEntity, Sprite(layerTexture, 1, 1, 0, z_index));
        }
    }

    printf("[INFO]: Tilemap sprite entity generated successfully\n");

    m_layers.clear();
}

void Tilemap::initCollisionMap() 
{
    printf("[INFO]: Initializing collision map\n");
    m_collision.resize(m_mapHeight);
    for (size_t row = 0; row < m_mapHeight; row++)
    {
        m_collision[row].resize(m_mapWidth, false);
    }
}

void Tilemap::generateCollisionTiles()
{
    printf("[INFO]: Generating collision tiles\n");
    // Loop through each row to identify contiguous blocks of collidable tiles
    for (size_t row = 0; row < m_mapHeight; row++)
    {
        size_t col = 0;

        while (col < m_mapWidth)
        {
            // Start of a collidable block
            if (m_collision[row][col])
            {
                size_t startCol = col;

                // End of the collidable block horizontally
                while (col < m_mapWidth && m_collision[row][col])
                {
                    col++;
                }

                size_t endCol = col;

                // Extend the block vertically
                size_t endRow = row;
                bool isCollidable = true;

                while (isCollidable && endRow + 1 < m_mapHeight)
                {
                    // Check if the next row has a collidable block
                    for (size_t currentCol = startCol; currentCol < endCol; currentCol++)
                    {
                        if (!m_collision[endRow + 1][currentCol])
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

                
                float w = (float)(endCol - startCol) * m_tilesize;
                float h = (float)(endRow - row + 1) * m_tilesize;

                float x = (float)startCol * m_tilesize * m_tileScale;
                float y = (float)row * m_tilesize * m_tileScale;

                EntityID tile = ECS::CreateEntity();
                ECS::AddComponent(tile, Transform(Vector2(x, y), 0.0, m_tileScale));
                ECS::AddComponent(tile, Collider(Vector2(w / 2.0f, h / 2.0f), w, h, true, true));
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
    for (size_t row = 0; row < m_mapHeight; row++) 
    {
        for (size_t col = 0; col < m_mapWidth; col++) 
        {
            m_collision[row][col] = std::find(tiles.begin(), tiles.end(), m_texture[layer][row][col]) != tiles.end();
        }
    }

    generateCollisionTiles();
    printf("[INFO]: Collision map generated succesfully\n");

    m_collision.clear();
    m_texture.clear();
}