#pragma once

#include "BaseSystem.hpp"
#include "../ECS.hpp"

struct SpriteSystem : BaseSystem
{
    void update(ECS& ecs)
    {
        // Update and animate all sprites
        for (const EntityID e : entities)
        {
            // Get transform and sprite components
            auto* transform = ecs.GetComponent<Transform>(e);
            auto* sprite = ecs.GetComponent<Sprite>(e);

            // Get the full width and height of the texture
            int textureWidth, textureHeight;
            SDL_QueryTexture(sprite->texture, NULL, NULL, &textureWidth, &textureHeight);

            // Calculate the width of each frame
            int frameWidth = textureWidth / sprite->h_frames;
            int frameHeight = textureHeight / sprite->v_frames;

            if (sprite->h_frames > 1 && sprite->speed > 0)
            {
                // Calculate current frame based on elapsed time
                sprite->h_frame = (SDL_GetTicks() / (1000 / sprite->speed)) % sprite->h_frames;

                // Set source rect to the current frame
                sprite->src.x = frameWidth * sprite->h_frame;
            }

            sprite->src.y = sprite->v_frame * frameHeight;  // Assuming all frames are in a single row
            sprite->src.w = frameWidth;
            sprite->src.h = frameHeight;

            // Set destination position and size based on transform and frame dimensions
            sprite->dst.x = static_cast<int>(transform->position.x);
            sprite->dst.y = static_cast<int>(transform->position.y);
            sprite->dst.w = frameWidth * transform->scale;
            sprite->dst.h = frameHeight * transform->scale;
        }
    }

    // Render all sprites
    void render(ECS& ecs, SDL_Renderer* renderer)
    {
        for (const EntityID e : entities)
        {
            auto* sprite = ecs.GetComponent<Sprite>(e);
            SDL_RenderCopy(renderer, sprite->texture, &sprite->src, &sprite->dst);
        }
    }
};


