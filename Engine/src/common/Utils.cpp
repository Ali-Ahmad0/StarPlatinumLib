#include "Utils.hpp"

// Color constants
const Color Color::RED	 = { 255, 0, 0 };
const Color Color::GREEN = { 0, 255, 0 };
const Color Color::BLUE  = { 0, 0, 255 };
const Color Color::BLACK = { 0, 0, 0 };
const Color Color::WHITE = { 255, 255, 255 };

// Vector2 constants
const Vector2 Vector2::ZERO  = { 0,  0 };
const Vector2 Vector2::UP    = { 0, -1 };
const Vector2 Vector2::LEFT  = { -1, 0 };
const Vector2 Vector2::DOWN  = {  0, 1 };
const Vector2 Vector2::RIGHT = {  1, 0 };

// Vector2 matrix transformation
Vector2 Vector2::transform(const Matrix3x2& matrix)
{
    return 
    {
        x * matrix.m11 + y * matrix.m21 + matrix.m31,
        x * matrix.m12 + y * matrix.m22 + matrix.m32
    };
}