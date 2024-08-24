#include "TextureManager.hpp"

SDL_Texture* SpriteManager::LoadTexture(const char* path, SDL_Renderer* renderer)
{
	// Load image as texture by using a temporary surface
	SDL_Surface* tmpSurface = IMG_Load(path);
	if (!tmpSurface) 
	{
		printf("Unable to load image: %s\n", IMG_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	SDL_FreeSurface(tmpSurface);
	return texture;
}
