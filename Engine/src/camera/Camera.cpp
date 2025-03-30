#include "Camera.hpp"
#include <iostream>

std::array<float, 4> Camera::boundaries = { -(float)_HUGE_ENUF, (float)(_HUGE_ENUF), -(float)_HUGE_ENUF, (float)_HUGE_ENUF };
Vector2 Camera::offset = { 0, 0 };

// Set offset if within boundaries
void Camera::SetOffset(const Vector2& newOffset) 
{
    // Clamp x to left and right boundaries
    offset.x = std::max(boundaries[0], std::min(newOffset.x, boundaries[1]));

    // Clamp y to up and down boundaries
    offset.y = std::max(boundaries[2], std::min(newOffset.y, boundaries[3]));
}

// Return camera offset
Vector2& Camera::GetOffset() 
{
    return offset;
}