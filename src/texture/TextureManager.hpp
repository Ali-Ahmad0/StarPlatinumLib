#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include "../main/Game.hpp"

class SpriteManager
{

public:
	static SDL_Texture* LoadTexture(const char* path, SDL_Renderer* renderer);
};

#endif // !SPRITE_MANAGER_HPP

