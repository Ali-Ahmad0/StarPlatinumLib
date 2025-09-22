#pragma once
#include "../../common/Utils.hpp"

// Will keep on adding stuff to this as needed
class Debug
{
public:
	static void DrawCircle(const Vector2& center, float r, 
		const Color& color = Color::WHITE, bool fill = true);
	
	static void DrawRect(const Vector2& center, float w, float h, float rotation = 0, 
		const Color& color = Color::WHITE, bool fill = true);

	static void ShowFramesPerSecond(const char* font, const Vector2& position, double delta, const Color& color = Color::WHITE);
	static void DisplayEntityCount(const char* font, const Vector2& position, double delta, const Color& color = Color::WHITE);

	// Flags
	static bool showCollisionShapes;
};
