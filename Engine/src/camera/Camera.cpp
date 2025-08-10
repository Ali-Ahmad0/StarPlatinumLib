#include <iostream>
#include "Camera.hpp"
#include "../viewport/ViewPort.hpp"

std::array<float, 4> Camera::s_boundaries = { -(float)_HUGE_ENUF, (float)(_HUGE_ENUF), -(float)_HUGE_ENUF, (float)_HUGE_ENUF };
Vector2 Camera::s_offset = { 0, 0 };

// Set offset within boundaries
void Camera::SetOffset(const Vector2& newOffset)
{
    // Get viewport size
    int viewPortWidth;
    int viewPortHeight;
    ViewPort::GetSize(&viewPortWidth, &viewPortHeight);

    // Calculate the camera bounds
    float cameraLeft = newOffset.x;
    float cameraRight = newOffset.x + (float)viewPortWidth;
    float cameraTop = newOffset.y;
    float cameraBottom = newOffset.y + (float)viewPortHeight;

    Vector2 clampedOffset = newOffset;

    // Check left boundary
    if (cameraLeft < s_boundaries[0]) 
    {
        clampedOffset.x = s_boundaries[0];
    }

    // Check right boundary  
    if (cameraRight > s_boundaries[1]) 
    {
        clampedOffset.x = s_boundaries[1] - (float)viewPortWidth;
    }

    // Check top boundary
    if (cameraTop < s_boundaries[2]) 
    {
        clampedOffset.y = s_boundaries[2];
    }

    // Check bottom boundary
    if (cameraBottom > s_boundaries[3]) 
    {
        clampedOffset.y = s_boundaries[3] - (float)viewPortHeight;
    }

    // Set the clamped offset
    s_offset = clampedOffset;
}

// Return camera offset
Vector2& Camera::GetOffset() 
{
    return s_offset;
}

void Camera::SetBoundaries(float left, float right, float up, float down)
{
    if (left > right)
        std::swap(left, right);

    if (up > down)
        std::swap(up, down);

    // Get viewport size
    int viewPortWidth;
    int viewPortHeight;
    ViewPort::GetSize(&viewPortWidth, &viewPortHeight);

    // Make sure boundaries are not less than viewport
    float boundaryWidth = right - left;
    float boundaryHeight = down - up;

    if (boundaryWidth < (float)viewPortWidth)
    {
        float padding = ((float)viewPortWidth - boundaryWidth) * 0.5f;
        left -= padding;
        right += padding;
    }

    if (boundaryHeight < (float)viewPortHeight)
    {
        float padding = ((float)viewPortHeight - boundaryHeight) * 0.5f;
        up -= padding;
        down += padding;
    }

    s_boundaries = { left, right, up, down };
}
