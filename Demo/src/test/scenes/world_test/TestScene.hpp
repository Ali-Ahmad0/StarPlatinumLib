#pragma once
#include "scene/SceneManager.hpp"
#include "texture/TextureManager.hpp"
#include "tilemap/Tilemap.hpp"

#include "TestPlayer.hpp"

class TestScene : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;
	void Events(SDL_Event event) override;

private:
	TestPlayer player;
};