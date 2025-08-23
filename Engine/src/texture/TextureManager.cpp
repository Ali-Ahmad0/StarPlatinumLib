#include "TextureManager.hpp"

std::vector<SDL_Texture*> TextureManager::s_textures{};

SDL_Texture* TextureManager::LoadTexture(const char* path)
{
	// Load image as texture by using a temporary surface
	printf("[INFO]: Loading image: %s as texture\n", path);
	SDL_Surface* tmpSurface = IMG_Load(path);
	if (!tmpSurface) 
	{
		fprintf(stderr, "[ERROR]: Unable to load image - %s\n", IMG_GetError());
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(ViewPort::GetRenderer(), tmpSurface);
	printf("[INFO]: Texture loaded successfully!\n");

	SDL_FreeSurface(tmpSurface);
	s_textures.push_back(texture);

	return texture;
}

void TextureManager::DestroyTexture(SDL_Texture* texture)
{
	if (!texture) 
	{
		return;
	}
	SDL_DestroyTexture(texture);
}

void TextureManager::Cleanup() 
{
	printf("[INFO]: Cleaning up textures\n");
	for (auto& texture : s_textures) 
	{
		DestroyTexture(texture);
	}
	printf("[INFO]: All textures cleaned up successfully\n");
}