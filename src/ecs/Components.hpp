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
    Uint32 h_frame; // Current horizontal frame
    Uint32 v_frame; // Current vertical frame

    Uint32 h_frames; // Total frames in one row
    Uint32 v_frames; // Total frames in one column

    Uint32 speed; // Speed in frames per second

    SpriteComponent(SDL_Texture* texture, int h_frames = 1, int v_frames = 1, int speed = 0)
        : texture(texture), src({ 0, 0, 0, 0 }), dst({ 0, 0, 0, 0 }), 
          h_frame(1), v_frame(0), h_frames(h_frames), v_frames(v_frames), speed(speed) {}
};

#endif // !COMPONENTS_HPP
