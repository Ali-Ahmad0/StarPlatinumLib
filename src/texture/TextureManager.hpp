#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "../common/Utils.hpp"

class TextureManager
{

public:
	// Load an image as SDL texture using path
	static SDL_Texture* LoadTexture(const char* path, SDL_Renderer* renderer);
};


