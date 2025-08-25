#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "../common/Utils.hpp"
#include "../main/Engine.hpp"

class TextureManager
{

public:
	// Load an image as SDL texture using path
	static SDL_Texture* LoadTexture(const char* path);
	static void DestroyTexture(SDL_Texture* texture);
	static void Cleanup();

private:
	static std::vector<SDL_Texture*> s_textures;
};