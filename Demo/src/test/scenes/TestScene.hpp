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

private:
	TestPlayer player;
	Tilemap tilemap = Tilemap(Vector2(0, 0), 16, 4);
};