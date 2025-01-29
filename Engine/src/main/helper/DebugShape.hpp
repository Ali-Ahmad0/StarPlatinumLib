#pragma once
#include <SDL.h>

class DebugShape 
{
public:
	static void DrawRect(const Vector2& position, float w, float h, const Color& Color = Color(255, 255, 255));
	static void DrawCircle();

};