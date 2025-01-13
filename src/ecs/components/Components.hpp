#pragma once

#include "../../common/Utils.hpp"
#include <SDL.h>

struct Transform
{
    // Transformation in a 2D plane
    Vector2 position; float rotation; size_t scale;

    Transform(const Vector2& position=Vector2(0, 0), float rotation=0.0,size_t scale = 1)
        : position(position), rotation(rotation), scale(scale) {}
};

struct Sprite
{
    SDL_Texture* texture;

    // Source and destination rect
    SDL_Rect src;
    SDL_Rect dst;

    size_t frame;

    size_t hframes; // Total frames in one row
    size_t vframes; // Total frames in one column

    size_t speed; // Speed in frames per second

    int8_t z_index; // Sprite layer

    // Map of animations
    std::unordered_map<std::string, std::vector<size_t>> animations{};
    const char* animation = "none";

    bool fliph = false;
    bool flipv = false;

    Sprite(SDL_Texture* texture=nullptr, size_t hframes = 1, size_t vframes = 1, size_t speed = 0)
        : texture(texture), src({ 0, 0, 0, 0 }), dst({ 0, 0, 0, 0 }), 
          frame(0), hframes(hframes), vframes(vframes), speed(speed) 
    {
        animations.insert({ "none", {} });
    }

    void addAnim(const char* anim, const std::vector<size_t>& frames) 
    {
        if (animations.find(anim) != animations.end()) 
        {
            std::cout << "Animation '" << anim << "' already added\n";
            return;
        }
        if (frames.empty()) 
        {
            std::cout << "Animation '" << anim << "' cannot have empty frame list\n";
            return;
        }
        // Add animation
        animations[anim] = frames;
    }

    void setAnim(const char* anim) 
    {
        if (animations.find(anim) == animations.end())
        {
            std::cout << "Animation '" << anim << "' does not exist\n";
            return;
        }

        // Set current animation
        animation = anim;
    }
       
    void delAnim(const char* anim)
    {
        if (animations.find(anim) == animations.end()) 
        {
            std::cout << "Animation '" << anim << "' does not exist\n";
            return;
        }
        // Remove animation
        animations.erase(anim);
    }
};

struct Movement 
{
    Vector2 direction;
    Vector2 speed;

    Movement(const Vector2& direction=Vector2(0, 0), const Vector2& speed=Vector2(0, 0)) : direction(direction), speed(speed) {}
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
        bool isSolid=true, bool isRigid=true)
        : center(Vector2(cx, cy)), dimensions(Vector2(w, h)), isSolid(isSolid), isRigid(isRigid)
    {
        min = Vector2(-w / 2, -h / 2);
        max = Vector2(w / 2, h / 2);
    }

    AABB(const Vector2& center=Vector2(0, 0), const Vector2& dimensions=Vector2(0, 0),
        bool isSolid=true, bool isRigid=true)
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

    bool colliding() 
    {
        return isColliding;
    }

private:
    bool isColliding = false;
};

