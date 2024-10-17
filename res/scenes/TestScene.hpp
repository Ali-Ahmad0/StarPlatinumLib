#pragma once
#include "../../src/scene/SceneManager.hpp"
#include "../../src/texture/TextureManager.hpp"
#include "../../src/tilemap/Tilemap.hpp"

class TestScene : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;
	void Events(SDL_Event event) override;
	void Draw() override;

private:
	EntityID player;
	EntityID player2;

	SDL_Texture* playerTexture;
	SDL_Texture* playerPreview;

	Tilemap tilemap;
};