#include "Debug.hpp"
#include <SDL.h>
#include <vector>

#include "../../viewport/ViewPort.hpp"
#include "../../camera/Camera.hpp"

void Debug::DrawCircle(const Vector2& center, float r, const Color& color, bool fill)
{
    // Set the draw color
    SDL_SetRenderDrawColor(ViewPort::GetRenderer(), color.r, color.g, color.b, color.a);

    // Use the function parameters
    int centerX = (int)(center.x - Camera::GetOffset().x);
    int centerY = (int)(center.y - Camera::GetOffset().y);
    int radius = (int)r;

    if (fill) 
    {
        // Draw filled circle using multiple horizontal lines
        for (int y = -radius; y <= radius; y++) 
        {
            int x = (int)sqrt(radius * radius - y * y);
            SDL_RenderDrawLine(ViewPort::GetRenderer(), centerX - x, centerY + y, centerX + x, centerY + y);
        }
    }
    else 
    {
        // Draw circle outline using Bresenham's circle algorithm
        int x = 0;
        int y = radius;
        int d = 3 - 2 * radius;

        while (x <= y) 
        {
            SDL_RenderDrawPoint(ViewPort::GetRenderer(), centerX + x, centerY + y);
            SDL_RenderDrawPoint(ViewPort::GetRenderer(), centerX - x, centerY + y);
            SDL_RenderDrawPoint(ViewPort::GetRenderer(), centerX + x, centerY - y);
            SDL_RenderDrawPoint(ViewPort::GetRenderer(), centerX - x, centerY - y);
            SDL_RenderDrawPoint(ViewPort::GetRenderer(), centerX + y, centerY + x);
            SDL_RenderDrawPoint(ViewPort::GetRenderer(), centerX - y, centerY + x);
            SDL_RenderDrawPoint(ViewPort::GetRenderer(), centerX + y, centerY - x);
            SDL_RenderDrawPoint(ViewPort::GetRenderer(), centerX - y, centerY - x);

            if (d < 0) 
            {
                d = d + 4 * x + 6;
            }
            else 
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }

    SDL_SetRenderDrawColor(ViewPort::GetRenderer(), 0, 0, 0, 255);
}

void Debug::DrawRect(const Vector2& position, float w, float h, float rotation, const Color& color, bool fill)
{
    SDL_Renderer* renderer = ViewPort::GetRenderer();
    if (!renderer) return;

    // Set render color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (rotation == 0.0f) 
    {
        // Simple case - no rotation
        SDL_Rect rect = 
        {
            (int)((position.x - w * 0.5f) - Camera::GetOffset().x),
            (int)((position.y - h * 0.5f) - Camera::GetOffset().y),
            (int)(w),
            (int)(h)
        };

        if (fill) 
        {
            SDL_RenderFillRect(renderer, &rect);
        }
        else 
        {
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
    else 
    {
        // Rotated rectangle - calculate corner points
        float halfW = w * 0.5f;
        float halfH = h * 0.5f;
        float radians = rotation * ((float)M_PI / 180.0f);
        float cosR = cos(radians);
        float sinR = sin(radians);

        // Calculate the four corners relative to center
        Vector2 corners[4] = 
        {
            { -halfW, -halfH }, // Top-left
            {  halfW, -halfH }, // Top-right
            {  halfW,  halfH }, // Bottom-right
            { -halfW,  halfH }  // Bottom-left
        };

        // Rotate and translate corners
        SDL_Point points[5]; // 5 points to close the rectangle
        for (int i = 0; i < 4; i++) 
        {
            float rotatedX = corners[i].x * cosR - corners[i].y * sinR;
            float rotatedY = corners[i].x * sinR + corners[i].y * cosR;

            points[i].x = (int)((position.x + rotatedX) - Camera::GetOffset().x);
            points[i].y = (int)((position.y + rotatedY) - Camera::GetOffset().y);
        }
        points[4] = points[0]; // Close the rectangle

        if (fill) 
        {
            // For filled rotated rectangles, we need to draw multiple lines
            // This is a simple scanline fill approach
            // Note: For better performance, consider using SDL_RenderGeometry if available

            // Find bounding box
            int minX = points[0].x, maxX = points[0].x;
            int minY = points[0].y, maxY = points[0].y;
            for (int i = 1; i < 4; i++) 
            {
                minX = std::min(minX, points[i].x);
                maxX = std::max(maxX, points[i].x);
                minY = std::min(minY, points[i].y);
                maxY = std::max(maxY, points[i].y);
            }

            // Simple fill by drawing horizontal lines
            for (int y = minY; y <= maxY; y++) 
            {
                std::vector<int> intersections;

                // Find intersections with rectangle edges
                for (int i = 0; i < 4; i++) 
                {
                    int j = (i + 1) % 4;
                    int y1 = points[i].y, y2 = points[j].y;
                    int x1 = points[i].x, x2 = points[j].x;

                    if ((y1 <= y && y < y2) || (y2 <= y && y < y1)) 
                    {
                        if (y2 != y1) 
                        {
                            int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
                            intersections.push_back(x);
                        }
                    }
                }

                std::sort(intersections.begin(), intersections.end());

                // Draw lines between pairs of intersections
                for (size_t i = 0; i < intersections.size(); i += 2) 
                {
                    if (i + 1 < intersections.size()) 
                    {
                        SDL_RenderDrawLine(renderer, intersections[i], y, intersections[i + 1], y);
                    }
                }
            }
        }
        else 
        {
            // Draw outline
            SDL_RenderDrawLines(renderer, points, 5);
        }
    }

    SDL_SetRenderDrawColor(ViewPort::GetRenderer(), 0, 0, 0, 255);
}
