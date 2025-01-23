#pragma once

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>

enum Direction
{
    UP, LEFT, DOWN, RIGHT
};

struct Color
{
    int r;
    int g;
    int b;
    int a;

    // Constructor to initialize color values using RGB values
    Color(int red, int green, int blue, int alpha = 255)
        : r(clamp(red)), g(clamp(green)), b(clamp(blue)), a(clamp(alpha)) {}

    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;

    // Constructor to initialize color values using HSV values
    Color(const float h,const float s, const float v, const int alpha = 255)
    {
        // Temporary variables to store the normalized RGB values
        float r = 0, g = 0, b = 0;

        // Determine the hue sector and the fractional part of the hue
        int i = (int)(h * 6);
        float f = h * 6 - i;

        // Intermediate values used in the conversion
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        // Determine the RGB values based on the hue sector
        switch (i % 6)
        {
            // Hue in the range [0, 60) degrees
            case 0: 
                r = v, g = t, b = p; 
                break; 
            
            // Hue in the range [60, 120) degrees
            case 1: 
                r = q, g = v, b = p; 
                break; 
            
            // Hue in the range [120, 180) degrees
            case 2: 
                r = p, g = v, b = t; 
                break; 
            
            // Hue in the range [180, 240) degrees
            case 3: 
                r = p, g = q, b = v; 
                break; 
            
            // Hue in the range [240, 300) degrees
            case 4:
                r = t, g = p, b = v;
                break; 
            

            // Hue in the range [300, 360) degrees
            case 5: 
                r = v, g = p, b = q; break; 
            
            default: 
                break;
        }

        // Convert the normalized RGB values (0.0 to 1.0) to 8-bit values (0 to 255)
        this->r = clamp((int)(r * 255));
        this->g = clamp((int)(g * 255));
        this->b = clamp((int)(b * 255));

        // Clamp alpha to the range 0 to 255
        this->a = clamp(alpha);  
    }

    // Constructor to initialize color values using a hex code
    Color(const std::string& hexCode)
    {
        // Remove the hash if it exists
        std::string hex = hexCode;
        if (hex[0] == '#')
        {
            hex = hex.substr(1);
        }

        // Parse the hex code
        switch (hex.length())
        {
        case 6:
            // Format is #RRGGBB
            r = std::stoi(hex.substr(0, 2), nullptr, 16);
            g = std::stoi(hex.substr(2, 2), nullptr, 16);
            b = std::stoi(hex.substr(4, 2), nullptr, 16);
            a = 255; // Default alpha value

            break;

        case 8:
            // Format is #RRGGBBAA
            r = std::stoi(hex.substr(0, 2), nullptr, 16);
            g = std::stoi(hex.substr(2, 2), nullptr, 16);
            b = std::stoi(hex.substr(4, 2), nullptr, 16);
            a = std::stoi(hex.substr(6, 2), nullptr, 16);

            break;

        default:
            // Invalid format, default to black
            r = 0;
            g = 0;
            b = 0;
            a = 255;
            break;
        }

        // Clamp the values to ensure they are in the range 0 to 255
        r = clamp(r);
        g = clamp(g);
        b = clamp(b);
        a = clamp(a);
    }

private:
    // Clamp values between 0 and 255
    static int clamp(const int value)
    {
        return std::max(0, std::min(255, value));
    }
};

// Stores information for a 2d vector
struct Vector2
{
    // X and Y values
    float x;
    float y;

    Vector2(float x=0, float y=0) : x(x), y(y) {}

    static const Vector2 ZERO;
    static const Vector2 UP;
    static const Vector2 LEFT;
    static const Vector2 DOWN;
    static const Vector2 RIGHT;

    // Add two vectors
    static Vector2 add(const Vector2& a, const Vector2& b)
    {
        return { a.x + b.x, a.y + b.y };
    }

    Vector2 add(const Vector2& b)
    {
        return { x + b.x, y + b.y };
    }

    // Subtract two vectors
    static Vector2 subtract(const Vector2& a, const Vector2& b)
    {
        return { a.x - b.x, a.y - b.y };
    }

    Vector2 subtract(const Vector2& b)
    {
        return { x - b.x, y - b.y };
    }

    // Multiply the vector by a number
    static Vector2 multiply(const Vector2& a, const float value)
    {
        return { a.x * value, a.y * value };
    }

    // Multiply the vector by a number
    Vector2 multiply(float value)
    {
        return { x * value, y * value };
    }

    // Divide the vector by a number
    static Vector2 divide(const Vector2& a, float value)
    {
        return { a.x / value, a.y / value };
    }

    Vector2 divide(const float value)
    {
        return { x / value, y / value };
    }

    // Dot product of two vectors
    static float dot(const Vector2& a, const Vector2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    Vector2 dot(const Vector2& b)
    {
        return { x * b.x, y * b.y };
    }

    // Get length squared of a vector
    static float magnitudeSquared(const Vector2& a)
    {
        return a.x * a.x + a.y * a.y;
    }

    float magnitudeSquared()
    {
        return x * x + y * y;
    }

    // Get length of vector
    static float magnitude(const Vector2& a)
    {
        return std::sqrt(magnitudeSquared(a));
    }

    float magnitude()
    {
        return std::sqrt(magnitudeSquared());
    }

    // Convert into unit vector
    static Vector2 normalize(const Vector2& a)
    {
        const float length = magnitude(a);

        if (length == 0)
        {
            return { 0, 0 };
        }

        return { a.x / length, a.y / length };
    }

    // Convert into unit vector
    Vector2 normalize()
    {
        const float length = magnitude();

        if (length == 0)
        {
            return { 0, 0 };
        }

        return { x / length, y / length };
    }

    // Check if 2 vectors are equal
    static bool equals(const Vector2& a, const Vector2& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    bool equals(const Vector2& b) const
    {
        return x == b.x && y == b.y;
    }

    // Operator overload
    // Arithmetic operators
    Vector2 operator+(const Vector2& b) 
    {
        return this->add(b);
    }

    Vector2& operator+=(const Vector2& b) 
    {
        x += b.x;
        y += b.y;

        return *this;
    }

    Vector2 operator-(const Vector2& b) 
    {
        return this->subtract(b);
    }

    Vector2& operator-=(const Vector2& b)
    {
        x -= b.x;
        y -= b.y;
        return *this;
    }

    Vector2 operator*(float value)
    {
        return this->multiply(value);
    }

    Vector2& operator*=(float value)
    {
        x *= value;
        y *= value;
        return *this;
    }

    Vector2 operator/(float value)
    {
        return this->divide(value);
    }

    Vector2& operator/=(float value)
    {
        x /= value;
        y /= value;
        return *this;
    }

    // Comparison operators
    bool operator==(const Vector2& b)
    {
        return this->equals(b);
    }

    bool operator!=(const Vector2& b)
    {
        return !this->equals(b);
    }

    bool operator>(const Vector2& b) 
    {
        return this->magnitudeSquared() > Vector2::magnitudeSquared(b);
    }
    
    bool operator>=(const Vector2& b) 
    {
        return this->magnitudeSquared() >= Vector2::magnitudeSquared(b);
    }

    bool operator<(const Vector2& b) 
    {
        return this->magnitudeSquared() < Vector2::magnitudeSquared(b);
    }

    bool operator<=(const Vector2& b) 
    {
        return this->magnitudeSquared() <= Vector2::magnitudeSquared(b);
    }

};

struct Rectangle
{
    // Position
    float x;
    float y;

    // Dimensions
    float w;
    float h;

    Rectangle(float x=0, float y=0, float w=0, float h=0) 
        : x(x), y(y), w(w), h(h) {}
    
    Rectangle(const Vector2& position=Vector2(0, 0), float w=0, float h=0)
        : x (position.x), y(position.y), w(w), h(h) {}
};