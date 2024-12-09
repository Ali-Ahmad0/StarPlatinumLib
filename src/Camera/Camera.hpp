// Camera.hpp
#pragma once
#include "../common/Utils.hpp"

class Camera {
public:
    Camera(const Vector2& position, const Vector2& dimensions, const Vector2& boundaries);

    void SetPosition(const Vector2& position);
    Vector2 GetPosition() const;

    void SetDimensions(const Vector2& dimensions);
    Vector2 GetDimensions() const;

    void SetBoundaries(const Vector2& boundaries);
    Vector2 GetBoundaries() const;

    void Move(const Vector2& delta);
    void Update(const Vector2& playerPosition); // New method to update camera position based on player

private:
    Vector2 position;
    Vector2 dimensions;
    Vector2 boundaries;

    void ClampPosition();
};
