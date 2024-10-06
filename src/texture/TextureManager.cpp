#include "TextureManager.hpp"


SDL_Texture* TextureManager::LoadTexture(const char* path)
{
	// Load image as texture by using a temporary surface
	SDL_Surface* tmpSurface = IMG_Load(path);
	if (!tmpSurface) 
	{
		printf("Unable to load image: %s\n", IMG_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetRenderer(), tmpSurface);

	SDL_FreeSurface(tmpSurface);
	return texture;
}
