#include "../ECS.hpp"

struct SpriteSystem
{

    void update(ECS& ecs)
    {
        for (EntityID e : ecs.activeEntityList)
        {
            if (ecs.hasComponent<TransformComponent>(e) && ecs.hasComponent<SpriteComponent>(e))
            {
                // Get transform and sprite components
                TransformComponent* transform = ecs.getComponent<TransformComponent>(e);
                SpriteComponent* sprite = ecs.getComponent<SpriteComponent>(e);

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
    }

    // Render all sprites
    void render(ECS& ecs, SDL_Renderer* renderer)
    {
        for (EntityID e : ecs.activeEntityList)
        {

            if (ecs.hasComponent<SpriteComponent>(e))
            {
                SpriteComponent* sprite = ecs.getComponent<SpriteComponent>(e);
                SDL_RenderCopy(renderer, sprite->texture, &sprite->src, &sprite->dst);
            }
        }
    }
};