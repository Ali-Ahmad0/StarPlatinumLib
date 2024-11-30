#pragma once

#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/texture/TextureManager.hpp"
#include "../../../src/tilemap/Tilemap.hpp"

#include "Player.hpp"

class MainScene : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;
	void Events(SDL_Event event) override;
	void Draw() override;

private:
	Player player;
	Tilemap tilemap = Tilemap(16, 4);
};
