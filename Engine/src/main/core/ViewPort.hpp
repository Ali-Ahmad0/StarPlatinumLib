#pragma once

#include <SDL.h>
#include "../../common/Utils.hpp"

class ViewPort 
{
public:
	static bool Init(const char* title = "Star Platinum Project", int w = 640, int h = 480, bool fullscreen = false,
		const Vector2& position = Vector2(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED));

	static void Exit();

	static SDL_Window* GetWindow();
	static SDL_Renderer* GetRenderer();

private:
	static SDL_Window* window;
	static SDL_Renderer* renderer;
};