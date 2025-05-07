#pragma once

#include <array>
#include <algorithm>
#include "../common/Utils.hpp"

class Camera 
{
public:
    static void SetOffset(const Vector2& offset);
    static Vector2& GetOffset();

    // left, right, up, down
    static std::array<float, 4> boundaries;
private:
    static Vector2 offset;
};
