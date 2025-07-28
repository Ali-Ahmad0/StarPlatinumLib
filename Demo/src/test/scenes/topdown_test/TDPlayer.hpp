#pragma once
#include "texture/TextureManager.hpp"

class TDPlayer
{
public:
	void Ready();
	void Update(double delta);
	
private:
	EntityID player;
	SDL_Texture* texture;

	Vector2 direction;

	Transform* transform;
	Sprite* sprite;
	Movement* movement;
};