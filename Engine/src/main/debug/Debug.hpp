#pragma once
#include "../../common/Utils.hpp"

// Will keep on adding stuff to this as needed
class Debug
{
public:
	static void DrawCircle(const Vector2& center, float r, 
		const Color& color = Color::WHITE, bool fill = true);
	
	static void DrawRect(const Vector2& position, float w, float h, 
		float rotation, const Color& color = Color::WHITE, bool fill = true);

};

