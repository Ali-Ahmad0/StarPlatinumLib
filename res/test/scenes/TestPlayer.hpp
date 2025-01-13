#pragma once
#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/texture/TextureManager.hpp"
#include "../../../src/tilemap/Tilemap.hpp"

class TestPlayer : public IScene 
{
public:
	void Ready() override;
	void Update(double delta) override;
	
private:
	EntityID player;
	SDL_Texture* texture;

	Vector2 direction;

	Transform* transform;
	Sprite* sprite;
	Movement* movement;
};