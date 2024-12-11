#include "Camera.hpp"

std::array<float, 4> Camera::boundaries = { -(float)_HUGE_ENUF, (float)(_HUGE_ENUF), -(float)_HUGE_ENUF, (float)_HUGE_ENUF };
Vector2 Camera::offset = Vector2(0, 0);

// Set offset if within boundaries
void Camera::SetOffset(const Vector2& newOffset) 
{
    offset = newOffset;
}

// Return camera offset
Vector2& Camera::GetOffset() 
{
    return offset;
}