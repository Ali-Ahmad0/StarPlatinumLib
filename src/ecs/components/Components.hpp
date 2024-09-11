#pragma once

#include "../../main/Game.hpp" 

struct Transform
{
    Vector2 position; // Position in a 2D space
    int scale; // Scale of entity

    explicit Transform(const Vector2 position=Vector2(0, 0), const int scale = 1)
        : position(position), scale(scale) { }
};

struct Sprite
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

    Sprite(SDL_Texture* texture, int h_frames = 1, int v_frames = 1, int speed = 0)
        : texture(texture), src({ 0, 0, 0, 0 }), dst({ 0, 0, 0, 0 }), 
          h_frame(1), v_frame(0), h_frames(h_frames), v_frames(v_frames), speed(speed) {}
};

