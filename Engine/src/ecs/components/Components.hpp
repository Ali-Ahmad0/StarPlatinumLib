#pragma once

#include "../../common/Utils.hpp"
#include <SDL.h>

struct Transform
{
    // Transformation in a 2D plane
    Vector2 position; float rotation; size_t scale;

    Transform(const Vector2& position = { 0, 0 }, float rotation = 0.0, size_t scale = 1)
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

    Sprite(SDL_Texture* texture = nullptr, size_t hframes = 1, size_t vframes = 1, size_t speed = 0, int8_t z_index = 0)
        : texture(texture), src({ 0, 0, 0, 0 }), dst({ 0, 0, 0, 0 }),
        frame(0), hframes(hframes), vframes(vframes), speed(speed), z_index(z_index)
    {
        animations.insert({ "none", {} });
    }

    void addAnim(const char* anim, const std::vector<size_t>& frames) 
    {
        if (animations.find(anim) != animations.end()) 
        {
            printf("[INFO]: Animation '%s' already added, overriding\n", anim);

            // Override animation
            animations[anim] = frames;
            return;
        }
        if (frames.empty()) 
        {
            fprintf(stderr, "[ERROR]: Animation '%s' cannot have empty frame list\n", anim);
            return;
        }

        // Add animation
        animations[anim] = frames;
    }

    void setAnim(const char* anim) 
    {
        if (animations.find(anim) == animations.end())
        {
            fprintf(stderr, "[ERROR]: Animation '%s' does not exist\n", anim);
            return;
        }

        // Set animation
        animation = anim;
    }
       
    void delAnim(const char* anim)
    {
        if (animations.find(anim) == animations.end()) 
        {
            fprintf(stderr, "[ERROR]: Animation '%s' does not exist\n", anim);
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

    Movement(const Vector2& direction = { 0, 0 }, const Vector2& speed = { 0, 0 }) : direction(direction), speed(speed) {}
};

struct Collider
{
    // Position and rotation
    Vector2 center;
    
    // Collision data
    Vector2 normal = Vector2::ZERO;
    float depth = 0;

    bool isColliding = false;

    // Circle collider constructor
    Collider(float cx, float cy, float r) : center(Vector2(cx, cy)), r(r), w(0), h(0), 
        shape(ShapeType::CIRCLE)
    {
        // Initialize the collider AABB
        aabb = AABB(cx, cy, r, r);
    }

    // Box collider constructor
    Collider(float cx, float cy, float w, float h) : center(Vector2(cx, cy)), r(0), w(w), h(h), 
        shape(ShapeType::BOX)
    {
        // Initialize the collider AABB
        aabb = AABB(cx, cy, w, h);
        
        // Initialize box vertices
        vertices[0] = Vector2(-w / 2,  h / 2); // Top left
        vertices[1] = Vector2( w / 2,  h / 2); // Top right
        vertices[2] = Vector2( w / 2, -h / 2); // Bottom right
        vertices[3] = Vector2(-w / 2, -h / 2); // Bottom left

        transformedVertices = vertices;
    }

    // Get updated vertices
    std::array<Vector2, 4>& getTransformedVertices(const Transform* transform)
    {
        // Return if no need to update vertices
        if (shape == ShapeType::CIRCLE)
            return transformedVertices;

        // Create a transformation matrix
        Matrix3x2 transformationMatrix = Matrix3x2::createRotation(transform->rotation) 
            * Matrix3x2::createTranslation(center + transform->position);

        // Update transformed vertices using the transformation matrix
        for (int i = 0; i < vertices.size(); i++) 
        {
            transformedVertices[i] = vertices[i].transform(transformationMatrix);
        }

        return transformedVertices;
    }

    // Get updated aabb
    AABB* getAABB(const Transform* transform)
    { 
        if (shape == ShapeType::BOX) 
        {
            const auto& vertices = getTransformedVertices(transform);

            // Find min and max position of edges using vertices
            float minX = std::numeric_limits<float>::infinity();
            float minY = std::numeric_limits<float>::infinity();
            float maxX = -std::numeric_limits<float>::infinity();
            float maxY = -std::numeric_limits<float>::infinity();

            for (const auto& vertex : vertices)
            {
                minX = std::min(minX, vertex.x);
                minY = std::min(minY, vertex.y);
                maxX = std::max(maxX, vertex.x);
                maxY = std::max(maxY, vertex.y);
            }

            // Update the passed AABB
            aabb.min = Vector2(minX, minY);
            aabb.max = Vector2(maxX, maxY);
        }
        else 
        {
            Vector2 worldCenter = center + transform->position;
            aabb.min = Vector2(worldCenter.x - r, worldCenter.y - r);
            aabb.max = Vector2(worldCenter.x + r, worldCenter.y + r);
        }

        return &aabb;
    }

    ShapeType getShape() { return shape; }

    float getRadius() { return r; }
    float getWidth() { return w; }
    float getHeight() { return h; }

private:
    // Dimensions
    float r;
    float w;
    float h;

    // Shape and bounding box
    ShapeType shape; AABB aabb;

    std::array<Vector2, 4> vertices{ Vector2::ZERO };
    std::array<Vector2, 4> transformedVertices{ Vector2::ZERO };
};

