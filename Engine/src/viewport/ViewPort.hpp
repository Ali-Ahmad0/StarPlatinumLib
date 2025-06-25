#pragma once

#include <SDL.h>
#include "../common/Utils.hpp"

class ViewPort 
{
public:
	static bool Init(const char* title, int w, int h, bool fullscreen);
	static void Exit();

	static SDL_Window* GetWindow();
	static SDL_Renderer* GetRenderer();

	static void GetSize(int* w, int* h);

private:
	static SDL_Window* window;
	static SDL_Renderer* renderer;
};