#include "TextureManager.hpp"


SDL_Texture* TextureManager::LoadTexture(const char* path)
{
	// Load image as texture by using a temporary surface
	printf("[INFO]: Loading image: %s as texture\n", path);
	SDL_Surface* tmpSurface = IMG_Load(path);
	if (!tmpSurface) 
	{
		fprintf(stderr, "[ERROR] Unable to load image: %s\n", IMG_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetRenderer(), tmpSurface);
	printf("[INFO]: Texture loadeds successfully!\n");

	SDL_FreeSurface(tmpSurface);
	return texture;
}
