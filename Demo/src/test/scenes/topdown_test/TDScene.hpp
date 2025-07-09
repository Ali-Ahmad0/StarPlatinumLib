#pragma once
#include "scene/SceneManager.hpp"
#include "texture/TextureManager.hpp"
#include "tilemap/Tilemap.hpp"

#include "TDPlayer.hpp"

class TDScene : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;
	void Events(SDL_Event event) override;

private:
	TDPlayer player;
};