// Camera.cpp
#include "Camera.hpp"

void Camera::Move(const Vector2& delta) 
{
    offset += delta;
}
