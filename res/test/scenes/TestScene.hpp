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
	void Events(SDL_Event event) override;
	void Draw() override;

private:
	TestPlayer player;

	SDL_Texture* playerPreview;
	Tilemap tilemap = Tilemap(16, 8);
};