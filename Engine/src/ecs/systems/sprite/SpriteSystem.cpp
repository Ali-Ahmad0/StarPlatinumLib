#include "SpriteSystem.hpp"
#include "../../ECS.hpp"
#include "../../../main/Engine.hpp"

#define CAMERA_MARGIN 32

void SpriteSystem::sortZ() 
{
    // Insertion sort entities based on z indices
    for (size_t i = 1; i < entities.size(); i++)
    {
        EntityID key = entities[i];
        auto* spriteA = ECS::GetComponent<Sprite>(key);

        int j = (int)i - 1;
        auto* spriteB = ECS::GetComponent<Sprite>(entities[j]);

        while (j >= 0 && spriteB->z_index > spriteA->z_index)
        {
            entities[j + 1] = entities[j];
            j--;
        }
        entities[j + 1] = key;
    }
}


void SpriteSystem::update(double delta) 
{
    // Get viewport and camera properties
    Vector2 cameraOffset = Camera::GetOffset();
    int screenWidth, screenHeight;
    ViewPort::GetSize(&screenWidth, &screenHeight);
    
    // Update and animate all sprites
    for (const EntityID e : entities)
    {
        auto* transform = ECS::GetComponent<Transform>(e);
        auto* sprite = ECS::GetComponent<Sprite>(e);

        // Get the full width and height of the texture
        int textureWidth, textureHeight;
        SDL_QueryTexture(sprite->texture, NULL, NULL, &textureWidth, &textureHeight);

        // Calculate the width of each frame
        size_t frameWidth = textureWidth / sprite->hframes;
        size_t frameHeight = textureHeight / sprite->vframes;

        // Skip rendering if sprite is not visible to camera
        if ((transform->position.x + frameWidth) < cameraOffset.x - CAMERA_MARGIN) continue;
        if (transform->position.x + CAMERA_MARGIN > (cameraOffset.x + screenWidth)) continue;
        if ((transform->position.y + frameHeight) < cameraOffset.y - CAMERA_MARGIN) continue;
        if (transform->position.y + CAMERA_MARGIN > (cameraOffset.y + screenHeight)) continue;

        // Animate the sprite
        if (sprite->animation != "none")
        {
            // Get list of frames to animate
            std::vector<size_t> frames = sprite->animations[sprite->animation];
            size_t frameIndex = (SDL_GetTicks() / (1000 / sprite->speed)) % frames.size();

            // Get current frame
            size_t frame = frames[frameIndex];

            if (frame >= sprite->hframes * sprite->vframes)
            {
                fprintf(stderr, "[ERROR] Invalid frame %zu in animation '%s'\n", sprite->frame, sprite->animation);
                continue;
            }

            // Update sprite frame
            sprite->frame = frame;

            // Calculate current horizontal and vertical frame
            size_t h_frame = sprite->frame % sprite->hframes;
            size_t v_frame = (sprite->frame / sprite->hframes) % sprite->vframes;

            // Calculate the position of src rectangle
            sprite->src.x = (int)(h_frame * frameWidth);
            sprite->src.y = (int)(v_frame * frameHeight);
        }

        // Set the source size based on frame dimensions
        sprite->src.w = (int)frameWidth;
        sprite->src.h = (int)frameHeight;

        // Set destination position and size based on transform and frame dimensions
        sprite->dst.x = (int)(transform->position.x - Camera::GetOffset().x);
        sprite->dst.y = (int)(transform->position.y - Camera::GetOffset().y);
        sprite->dst.w = (int)(frameWidth * transform->scale);
        sprite->dst.h = (int)(frameHeight * transform->scale);

        // Flip sprite
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (sprite->fliph) flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
        if (sprite->flipv) flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);

        // Render sprite
        SDL_RenderCopyEx(ViewPort::GetRenderer(), sprite->texture, &sprite->src, &sprite->dst, transform->rotation, NULL, flip);
    }
}

void SpriteSystem::onEntityAdded(EntityID e) 
{
    // Add the entity, sort the layers
    entities.push_back(e);
    sortZ();
}

void SpriteSystem::onEntityRemoved(EntityID e) 
{
    // Delete the entity, sort the layers
    auto position = std::find(entities.begin(), entities.end(), e);
    entities.erase(position);
    sortZ();
}