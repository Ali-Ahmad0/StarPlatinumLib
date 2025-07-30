#pragma once

#include <unordered_map>
#include <string>
#include <SDL_ttf.h>

#include "../common/Utils.hpp"

class TextManager
{
public:
	static void LoadFont(const char* name, const char* path, size_t size = 16);
	static void DrawText(const char* text, const char* font, const Vector2& position, const Color& color = Color::WHITE);
	static void Cleanup();

private:
	static std::unordered_map<std::string, TTF_Font*> fonts;
	static bool initialized;
};

