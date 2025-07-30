#include "Utils.hpp"

// Color constants
const Color Color::RED	 = { (uint8_t)255, (uint8_t)0, (uint8_t)0 };
const Color Color::GREEN = { (uint8_t)0, (uint8_t)255, (uint8_t)0 };
const Color Color::BLUE  = { (uint8_t)0, (uint8_t)0, (uint8_t)255 };
const Color Color::BLACK = { (uint8_t)0, (uint8_t)0, (uint8_t)0 };
const Color Color::WHITE = { (uint8_t)255, (uint8_t)255, (uint8_t)255 };

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