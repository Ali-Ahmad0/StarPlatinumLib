// Camera.cpp
#include "Camera.hpp"

Camera::Camera(const Vector2& position, const Vector2& dimensions, const Vector2& boundaries)
    : position(position), dimensions(dimensions), boundaries(boundaries) {
    ClampPosition();
}

void Camera::SetPosition(const Vector2& position) {
    this->position = position;
    ClampPosition();
}

Vector2 Camera::GetPosition() const {
    return position;
}

void Camera::SetDimensions(const Vector2& dimensions) {
    this->dimensions = dimensions;
}

Vector2 Camera::GetDimensions() const {
    return dimensions;
}

void Camera::SetBoundaries(const Vector2& boundaries) {
    // Ensure boundaries are not smaller than dimensions
    this->boundaries.x = std::max(boundaries.x, dimensions.x);
    this->boundaries.y = std::max(boundaries.y, dimensions.y);
    ClampPosition();
}

Vector2 Camera::GetBoundaries() const {
    return boundaries;
}

void Camera::Move(const Vector2& delta) {
    position = position + delta;
    ClampPosition();
}

void Camera::Update(const Vector2& playerPosition) {
    // Center the camera on the player
    position.x = playerPosition.x - dimensions.x / 2;
    position.y = playerPosition.y - dimensions.y / 2;
    ClampPosition();
}

void Camera::ClampPosition() {
    if (position.x < 0) position.x = 0;
    if (position.y < 0) position.y = 0;
    if (position.x + dimensions.x > boundaries.x) position.x = boundaries.x - dimensions.x;
    if (position.y + dimensions.y > boundaries.y) position.y = boundaries.y - dimensions.y;
}
