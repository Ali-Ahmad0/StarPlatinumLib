// Camera.hpp
#pragma once
#include <array>
#include "../common/Utils.hpp"


class Camera {
public:
    Camera(const Vector2& offset, const std::array<float, 4>& boundaries) : offset(offset), boundaries(boundaries) {}

    void Move(const Vector2& delta);
    Vector2 offset;

    // -ve x, +ve x, -ve y, +ve y
    std::array<float, 4> boundaries;
};
