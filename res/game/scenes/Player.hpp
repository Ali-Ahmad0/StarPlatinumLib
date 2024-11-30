#pragma once

#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/texture/TextureManager.hpp"

class Player : public IScene 
{
public:
	void Ready() override;
	void Update(double delta) override;
	void Events(SDL_Event event) override;
	void Draw() override;

private:
	EntityID player;
	SDL_Texture* playerTexture;

	// Components to modify
	Movement* movement;
	Sprite* sprite;
};