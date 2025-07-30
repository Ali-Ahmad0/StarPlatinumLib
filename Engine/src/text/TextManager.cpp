#include "TextManager.hpp"
#include <stdio.h>

#include "../viewport/ViewPort.hpp"

std::unordered_map<std::string, TTF_Font*> TextManager::fonts{};

void TextManager::LoadFont(const char* name, const char* path, size_t size)
{
    // Check if font is already loaded
    std::string fontKey = std::string(name);
    if (fonts.find(fontKey) != fonts.end()) 
    {
        printf("[INFO]: Font already loaded");
        return;
    }

    // Load the font
    TTF_Font* font = TTF_OpenFont(path, (int)(size));
    if (!font) 
    {
        fprintf(stderr, "[ERROR]: Error loading font - %s\n", TTF_GetError());
        return;
    }

    // Store the font in the unordered map
    fonts[fontKey] = font;
    printf("[INFO]: Font %s loaded successfully\n", name);
}

void TextManager::DrawText(const char* text, const char* font, const Vector2& position, const Color& color)
{
    auto it = fonts.find(std::string(font));
    if (it == fonts.end())
    {
        fprintf(stderr, "[ERROR]: Font %s not found. Make sure to load it first.\n", font);
        return;
    }

    TTF_Font* ttfFont = it->second;

    // Create SDL color
    SDL_Color sdlColor = {
        color.r,
        color.g,
        color.b,
        color.a
    };

    // Render text to surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(ttfFont, text, sdlColor);
    if (!textSurface)
    {
        fprintf(stderr, "[ERROR]: Failed to render text: %s\n", TTF_GetError());
        return;
    }

    // Create texture from surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ViewPort::GetRenderer(), textSurface);
    SDL_FreeSurface(textSurface);

    if (!textTexture)
    {
        fprintf(stderr, "[ERROR]: Failed to create texture: %s\n", SDL_GetError());
        return;
    }

    // Get texture dimensions
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Set destination rectangle
    SDL_Rect destRect = {
        (int)position.x,
        (int)position.y,
        textWidth,
        textHeight
    };

    // Render the texture
    SDL_RenderCopy(ViewPort::GetRenderer(), textTexture, NULL, &destRect);
    SDL_DestroyTexture(textTexture);
}

void TextManager::Cleanup()
{
    printf("[INFO]: Cleaning up fonts...\n");
    for (auto& pair : fonts)
    {
        TTF_CloseFont(pair.second);
        printf("[INFO]: Font %s cleaned up\n", pair.first.c_str());
    }
    fonts.clear();
    printf("[INFO]: All fonts cleaned up successfully\n");
}
