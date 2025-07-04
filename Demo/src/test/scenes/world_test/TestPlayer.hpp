#pragma once
#include "scene/SceneManager.hpp"
#include "texture/TextureManager.hpp"

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