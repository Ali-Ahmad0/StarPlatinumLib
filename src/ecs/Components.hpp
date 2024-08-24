#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "../main/Game.hpp" 

struct Component
{
    virtual ~Component() = default;
};

struct TransformComponent : public Component
{
    Vector2 position;
    int scale;

    TransformComponent(Vector2 position, int scale)
        : position(position), scale(scale) { }
};

struct SpriteComponent : public Component
{
    SDL_Texture* texture;

    // Source and destination rect
    SDL_Rect src;
    SDL_Rect dst;

    // In case of animated sprite
    Uint32 frame; // Current frame
    Uint32 frames; // Total frames

    Uint32 speed; // Speed in frames per second

    SpriteComponent(SDL_Texture* texture, int frames = 1, int speed = 0)
        : texture(texture), src({ 0, 0, 0, 0 }), dst({ 0, 0, 0, 0 }), frame(1), frames(frames), speed(speed) {}
};

#endif // !COMPONENTS_HPP
