#include <algorithm>
#include <cmath>
#include <sstream>   
#include <iomanip>   

struct Color
{
    int r;
    int g;
    int b;
    int a;

    // Constructor to initialize color values using RGB values
    Color(int red, int green, int blue, int alpha = 255)
        : r(clamp(red)), g(clamp(green)), b(clamp(blue)), a(clamp(alpha)) {}

    // Constructor to initialize color values using HSV values
    Color(float h, float s, float v, int alpha = 255)
    {
        // Temporary variables to store the normalized RGB values
        float r = 0, g = 0, b = 0;

        // Determine the hue sector and the fractional part of the hue
        int i = static_cast<int>(h * 6);
        float f = h * 6 - i;

        // Intermediate values used in the conversion
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        // Determine the RGB values based on the hue sector
        switch (i % 6)
        {
        case 0: r = v, g = t, b = p; break; // Hue in the range [0, 60) degrees
        case 1: r = q, g = v, b = p; break; // Hue in the range [60, 120) degrees
        case 2: r = p, g = v, b = t; break; // Hue in the range [120, 180) degrees
        case 3: r = p, g = q, b = v; break; // Hue in the range [180, 240) degrees
        case 4: r = t, g = p, b = v; break; // Hue in the range [240, 300) degrees
        case 5: r = v, g = p, b = q; break; // Hue in the range [300, 360) degrees
        }

        // Convert the normalized RGB values (0.0 to 1.0) to 8-bit values (0 to 255)
        this->r = clamp(static_cast<int>(r * 255));
        this->g = clamp(static_cast<int>(g * 255));
        this->b = clamp(static_cast<int>(b * 255));
        this->a = clamp(alpha);  // Clamp alpha to the range 0 to 255
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
    int clamp(int value) const
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

    Vector2(float xpos, float ypos) : x(xpos), y(ypos) {}

    // Add a vector
    Vector2 add(Vector2& b) const
    {
        return Vector2(x + b.x, y + b.y);
    }

    // Subtract a vector
    Vector2 subtract(Vector2& b) const
    {
        return Vector2(x - b.x, y - b.y);
    }

    // Scale the vector
    Vector2 scale(float value) const
    {
        return Vector2(x * value, y * value);
    }

    // Dot product of vector
    Vector2 dot(Vector2& b) const
    {
        return Vector2(x * b.x, y * b.y);
    }

    // Get length of vector
    float magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }

    // Convert into unit vector
    Vector2 normalize() const
    {
        float length = magnitude();

        if (length == 0)
        {
            return Vector2(0, 0);
        }

        return Vector2(x / length, y / length);
    }
};