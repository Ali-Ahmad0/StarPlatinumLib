#pragma once
#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/texture/TextureManager.hpp"

class StressTest : public IScene
{
public:
	void Ready() override;
	void Events(SDL_Event event) override;

private:
	SDL_Texture* texture;
};