#include "SpriteSystem.hpp"
#include "../../main/Game.hpp"

void SpriteSystem::update() 
{
    // Update and animate all sprites
    for (const EntityID e : entities)
    {
        // Get transform and sprite components
        auto* transform = ECS::GetComponent<Transform>(e);
        auto* sprite = ECS::GetComponent<Sprite>(e);

        // Get the full width and height of the texture
        int textureWidth, textureHeight;
        SDL_QueryTexture(sprite->texture, NULL, NULL, &textureWidth, &textureHeight);

        // Calculate the width of each frame
        size_t frameWidth = textureWidth / sprite->hframes;
        size_t frameHeight = textureHeight / sprite->vframes;

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
                std::cout << "Invalid frame in animation '" << sprite->animation << "'\n";
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
        SDL_RenderCopyEx(Engine::GetRenderer(), sprite->texture, &sprite->src, &sprite->dst, transform->rotation, NULL, flip);
    }
}