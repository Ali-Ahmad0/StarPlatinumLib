#pragma once

#include "../../common/Utils.hpp"
#include <SDL.h>

struct Transform
{
    Vector2 position; // Position in a 2D space
    int scale; // Scale of entity

    Transform(const Vector2 position=Vector2(0, 0), const int scale = 1)
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

    Sprite(SDL_Texture* texture=nullptr, int h_frames = 1, int v_frames = 1, int speed = 0)
        : texture(texture), src({ 0, 0, 0, 0 }), dst({ 0, 0, 0, 0 }), 
          h_frame(1), v_frame(0), h_frames(h_frames), v_frames(v_frames), speed(speed) {}
};

struct Movement 
{
    Vector2 direction;
    float scale;

    Movement(Vector2 direction=Vector2(0, 0), float scale = 0) : direction(direction), scale(scale) {}
};

struct AABB 
{
    // Size
    Vector2 dimensions;

    // Boundaries
    Vector2 min;
    Vector2 max;

    bool isSolid;
    bool isRigid;

    AABB(float w, float h, bool isSolid=false, bool isRigid=true)
        : dimensions(Vector2(w, h)), isSolid(isSolid), isRigid(isRigid)
    {
        min = Vector2(-w / 2, -h / 2);
        max = Vector2(w / 2, h / 2);
    }

    AABB(const Vector2& dimensions, bool isSolid=false, bool isRigid=true) 
        : dimensions(dimensions), isSolid(isSolid), isRigid(isRigid)
    {
        min = Vector2(-dimensions.x / 2, -dimensions.y / 2);
        max = Vector2(dimensions.x / 2, dimensions.y / 2);
    }

    // Check for AABB overlap
    bool check(AABB& other) 
    {
        // Set isColliding to true if there is overlap on both the X and Y axes
        isColliding = !(max.x < other.min.x || min.x > other.max.x || max.y < other.min.y || min.y > other.max.y);
        other.isColliding = isColliding;

        return isColliding;
    }

    // Return whether a collision has occurred
    bool colliding() 
    {
        return isColliding;
    }

private:
    bool isColliding = false;
};

