#pragma once

#include "Utils.hpp"
#include <SDL.h>

struct Transform
{
    // Transformation in a 2D plane
    Vector2 position; float rotation; size_t scale;

    Transform(const Vector2& position = Vector2::ZERO, float rotation = 0.0f, size_t scale = 1)
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

    // Map of animations
    std::unordered_map<std::string, SpriteAnimationData> animations{};

    bool fliph = false;
    bool flipv = false;

    float ySortOrigin = 0.0f;
    bool applyYSort = false;

    Sprite(SDL_Texture* texture = nullptr, size_t hframes = 1, size_t vframes = 1, int8_t zIndex = 0)
        : texture(texture), src({ 0, 0, 0, 0 }), dst({ 0, 0, 0, 0 }),
        frame(0), hframes(hframes), vframes(vframes), m_zIndex(zIndex)
    {
        // Initialize the default animation
        animations["none"] = SpriteAnimationData();

        // Set texture and frame dimensions
        SDL_QueryTexture(this->texture, NULL, NULL, &m_textureWidth, &m_textureHeight);
        m_frameWidth = (int)(m_textureWidth / hframes);
        m_frameHeight = (int)(m_textureHeight / vframes);
    }

    void AddAnimation(const char* animation, const std::vector<size_t>& frames, size_t speed) 
    {
        SpriteAnimationData data = SpriteAnimationData(frames, speed);

        if (animations.find(animation) != animations.end()) 
        {
            printf("[INFO]: Animation '%s' already added, overriding\n", animation);
            // Override animation
            animations[animation] = data;
            return;
        }
        if (frames.empty()) 
        {
            fprintf(stderr, "[ERROR]: Animation '%s' cannot have empty frame list\n", animation);
            return;
        }

        // Add animation
        animations[animation] = data;
    }

    void SetAnimation(const char* animation) 
    {
        if (animations.find(animation) == animations.end())
        {
            fprintf(stderr, "[ERROR]: Animation '%s' does not exist\n", animation);
            return;
        }

        // Set animation
        m_animation = animation;
    }
       
    void DelAnimation(const char* animation)
    {
        if (animations.find(animation) == animations.end()) 
        {
            fprintf(stderr, "[ERROR]: Animation '%s' does not exist\n", animation);
            return;
        }

        // Remove animation
        animations.erase(animation);
    }

    const char* GetAnimation() {
        return m_animation;
    }

    void SetZIndex(int8_t zIndex) 
    {
        m_zIndex = zIndex;
        m_zIndexChanged = true;
    }

    int8_t GetZIndex() 
    {
        return m_zIndex;
    }

    // Returns if Z Index was changed since the last time this function was called
    bool IsZIndexChanged() 
    {
        bool changed = m_zIndexChanged;
        m_zIndexChanged = false;
        return changed;
    }

    int GetTextureWidth() 
    {
        return m_textureWidth;
    }

    int GetTextureHeight() 
    {
        return m_textureHeight;
    }

    int GetFrameWidth() 
    {
        return m_frameWidth;
    }

    int GetFrameHeight() 
    {
        return m_frameHeight;
    }

private:
    // Current animation name
    const char* m_animation = "none";

    // Z Index properties
    int8_t m_zIndex = 0;
    bool m_zIndexChanged = false;

    // Texture and frame dimensions
    int m_textureWidth;
    int m_textureHeight;

    int m_frameWidth;
    int m_frameHeight;
};

struct Movement
{
    Vector2 direction;
    Vector2 speed;

    Movement(const Vector2& direction = { 0, 0 }, const Vector2& speed = { 0, 0 }) : direction(direction), speed(speed) {}
};

struct Collider
{
    // Offset of collider center from transform position
    Vector2 centerOffset;
    
    // Collision data
    Vector2 normal = Vector2::ZERO;
    bool isColliding = false;

    // Properties
    bool isStatic;
    bool isSolid;

    // Draw shape (for debugging)
    Color debugDrawColor = Color((uint8_t)100, (uint8_t)100, (uint8_t)255, (uint8_t)100);

    // Circle collider constructor
    Collider(const Vector2& centerOffset, float r, bool isSolid = true, bool isStatic = false)
        : centerOffset(centerOffset), m_r(r), m_w(0), m_h(0), 
        m_shape(ShapeType::CIRCLE), isSolid(isSolid), isStatic(isStatic)
    {
        // Initialize the collider AABB
        m_aabb = AABB(r, r);
    }

    // Box collider constructor
    Collider(const Vector2& centerOffset, float w, float h, bool isSolid = true, bool isStatic = false)
        : centerOffset(centerOffset), m_r(0), m_w(w), m_h(h), 
        m_shape(ShapeType::BOX), isSolid(isSolid), isStatic(isStatic)
    {
        // Initialize the collider AABB
        m_aabb = AABB(w, h);
        
        // Initialize box vertices
        m_vertices[0] = Vector2(-w / 2, -h / 2); // Bottom left
        m_vertices[1] = Vector2( w / 2, -h / 2); // Bottom right  
        m_vertices[2] = Vector2( w / 2, h / 2); // Top right
        m_vertices[3] = Vector2(-w / 2, h / 2); // Top left

        m_transformedVertices = m_vertices;
    }

    // Get updated vertices
    std::array<Vector2, 4>& GetTransformedVertices(const Transform* transform)
    {
        // Return if no need to update vertices
        if (m_shape == ShapeType::CIRCLE)
            return m_transformedVertices;

        float radians = transform->rotation * ((float)M_PI / 180.0f);

        // Create a transformation matrix
        Matrix3x2 transformationMatrix = 
            Matrix3x2::CreateScale(transform->scale) * 
            Matrix3x2::CreateRotation(radians) * 
            Matrix3x2::CreateTranslation(centerOffset * (float)transform->scale + transform->position);

        // Update transformed vertices using the transformation matrix
        for (int i = 0; i < m_vertices.size(); i++) 
        {
            m_transformedVertices[i] = m_vertices[i].Transform(transformationMatrix);
        }

        return m_transformedVertices;
    }

    // Get updated aabb
    AABB* GetAABB(const Transform* transform)
    { 
        if (m_shape == ShapeType::BOX) 
        {
            const auto& vertices = GetTransformedVertices(transform);

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
            m_aabb.min = Vector2(minX, minY);
            m_aabb.max = Vector2(maxX, maxY);
        }
        else 
        {
            Vector2 worldCenter = centerOffset + transform->position;
            m_aabb.min = Vector2(worldCenter.x - m_r, worldCenter.y - m_r);
            m_aabb.max = Vector2(worldCenter.x + m_r, worldCenter.y + m_r);
        }

        return &m_aabb;
    }

    ShapeType GetShape() 
    { 
        return m_shape; 
    }

    float GetRadius() 
    { 
        return m_r; 
    }
    
    float GetWidth() 
    { 
        return m_w; 
    }
    
    float GetHeight() 
    { 
        return m_h; 
    }

    bool IsOnFloor() 
    {
        return normal.y > 0.5f;
    }

    bool IsOnLeftWall() 
    {
        return normal.x > 0.5f;
    }

    bool IsOnRightWall()
    {
        return normal.x < -0.5f;
    }

    bool IsOnCeiling() 
    {
        return normal.y < -0.5f;
    }

private:
    // Dimensions
    float m_r;
    float m_w;
    float m_h;

    // Shape type
    ShapeType m_shape; 

    // Vertices 
    std::array<Vector2, 4> m_vertices{ Vector2::ZERO };
    std::array<Vector2, 4> m_transformedVertices{ Vector2::ZERO };
    
    // Bounding box
    AABB m_aabb;
};

struct VerletObject 
{
    Vector2 prevPosition;
    Vector2 acceleration;
    bool isStationary;

    VerletObject(const Vector2& position, float mass = 1.0f, bool isStationary = false) 
        : prevPosition(position), acceleration(Vector2::ZERO), isStationary(isStationary)
    {
        SetMass(mass);
    }

    // Apply a force
    void ApplyForce(const Vector2& amount) 
    {
        acceleration = Vector2::Divide(amount, m_mass);
    }

    float GetMass() 
    {
        return m_mass;
    }

    void SetMass(float mass) 
    {
        if (mass <= 0.0f)
        {
            throw std::runtime_error("[RUNTIME ERROR]: Mass must be positive");
        }
        this->m_mass = mass;
    }

private:
    float m_mass;
};