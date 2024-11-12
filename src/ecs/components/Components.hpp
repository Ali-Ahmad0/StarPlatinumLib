#pragma once

#include "../../common/Utils.hpp"
#include <SDL.h>

struct Transform
{
    Vector2 position; // Position in a 2D space
    size_t scale; // Scale of entity

    Transform(const Vector2& position=Vector2(0, 0), const size_t scale = 1)
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
    float speed;

    Movement(const Vector2& direction=Vector2(0, 0), float speed = 0) : direction(direction), speed(speed) {}
};

struct Gravity
{
    Direction direction;
    float g;

    Gravity(Direction direction=Direction::DOWN, float g=9.81f) : direction(direction), g(g) {}
};

struct AABB 
{
    // Center
    Vector2 center;

    // Size
    Vector2 dimensions;

    // Boundaries
    Vector2 min;
    Vector2 max;

    bool isSolid;
    bool isRigid;

    AABB(float cx, float cy, float w, float h, 
        bool isSolid=false, bool isRigid=true)
        : center(Vector2(cx, cy)), dimensions(Vector2(w, h)), isSolid(isSolid), isRigid(isRigid)
    {
        min = Vector2(-w / 2, -h / 2);
        max = Vector2(w / 2, h / 2);
    }

    AABB(const Vector2& center=Vector2(0, 0), const Vector2& dimensions= Vector2(0, 0),
        bool isSolid = false, bool isRigid = true)
        : center(center), dimensions(dimensions), isSolid(isSolid), isRigid(isRigid)
    {
        min = Vector2(-dimensions.x / 2, -dimensions.y / 2);
        max = Vector2(dimensions.x / 2, dimensions.y / 2);
    }

    // Check for AABB overlap
    bool intersects(AABB& other) 
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

