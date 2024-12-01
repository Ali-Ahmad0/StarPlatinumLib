#pragma once

#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/texture/TextureManager.hpp"

class Player : public IScene 
{
public:
	void Ready() override;
	void Update(double delta) override;

private:
	EntityID player;
	SDL_Texture* playerTexture;

	// Components to modify
	Movement* movement;
	Sprite* sprite;
};