#include "Debug.hpp"
#include <SDL.h>

#include "../../viewport/ViewPort.hpp"

void Debug::DrawCircle(const Vector2& center, float r, const Color& color, bool fill)
{
    // Set the draw color
    SDL_SetRenderDrawColor(ViewPort::GetRenderer(), color.r, color.g, color.b, color.a);

    // Use the function parameters
    int centerX = (int)center.x;
    int centerY = (int)center.y;
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

    SDL_SetRenderDrawColor(ViewPort::GetRenderer(), 0, 0, 0, 0);
}