#pragma once
#include "scene/Scene.hpp"
#include "texture/TextureManager.hpp"
#include "tilemap/Tilemap.hpp"

#include "TDPlayer.hpp"

class TDScene : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;

private:
	TDPlayer player;
};