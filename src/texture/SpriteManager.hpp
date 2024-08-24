#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include "../main/Game.hpp"

class SpriteManager
{

public:
	static SDL_Texture* LoadTexture(const char* path, SDL_Renderer* renderer);

	//// Draw a texture as a single image
	//static void DrawTexture(SDL_Renderer* renderer, const SpriteComponent& sprite);

	//// Draw animation with a spritesheet
	//static void DrawAnimation(SDL_Renderer* renderer, const AnimatedSpriteComponent& animatedSprite);
};

#endif // !SPRITE_MANAGER_HPP

