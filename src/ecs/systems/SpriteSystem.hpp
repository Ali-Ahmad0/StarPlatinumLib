#include "../ECS.hpp"

struct SpriteSystem
{
    void update(EntityManager& ecs)
    {
        // Update positions and animations for all entities with Transform and Sprite components
        Entity firstEntity = ecs.getNextEntity() - ecs.getEntityCount();

        for (Entity e = firstEntity; e < ecs.getNextEntity(); e++)
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
                int frameWidth = textureWidth / sprite->frames;
                int frameHeight = textureHeight;

                if (sprite->frames > 1 && sprite->speed > 0)
                {
                    // Calculate current frame based on elapsed time
                    sprite->frame = (SDL_GetTicks() / (1000 / sprite->speed)) % sprite->frames;
                    
                    // Set source rect to the current frame
                    sprite->src.x = frameWidth * sprite->frame;
                }

       
                sprite->src.y = 0;  // Assuming all frames are in a single row
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
    void render(EntityManager& ecs, SDL_Renderer* renderer)
    {
        Entity firstEntity = ecs.getNextEntity() - ecs.getEntityCount();

        for (Entity e = firstEntity; e < ecs.getNextEntity(); e++)
        {

            if (ecs.hasComponent<SpriteComponent>(e))
            {
                SpriteComponent* sprite = ecs.getComponent<SpriteComponent>(e);
                SDL_RenderCopy(renderer, sprite->texture, &sprite->src, &sprite->dst);
            }
        }
    }
};