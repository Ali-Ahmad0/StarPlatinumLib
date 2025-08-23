#pragma once

#include <array>
#include "../common/Utils.hpp"

class Camera 
{
public:
    static void SetOffset(const Vector2& offset);
    static Vector2& GetOffset();

    static void SetBoundaries(float left, float right, float up, float down);
private:
    // left, right, up, down
    static std::array<float, 4> s_boundaries;
    static Vector2 s_offset;
};
