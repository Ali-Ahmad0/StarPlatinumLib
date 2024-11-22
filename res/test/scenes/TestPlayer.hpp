#pragma once
#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/texture/TextureManager.hpp"
#include "../../../src/tilemap/Tilemap.hpp"

class TestPlayer : public IScene 
{
public:
	void Ready() override;
	void Events(SDL_Event event) override;
	
private:
	EntityID player;
	SDL_Texture* texture;

	Movement* movement;
	Sprite* sprite;
};