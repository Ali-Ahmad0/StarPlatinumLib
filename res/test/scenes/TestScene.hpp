#pragma once
#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/texture/TextureManager.hpp"
#include "../../../src/tilemap/Tilemap.hpp"

#include "TestPlayer.hpp"

class TestScene : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;

private:
	TestPlayer player;

	SDL_Texture* playerPreview;
	Tilemap tilemap = Tilemap(Vector2(0, 0), 16, 4);
};