#pragma once
#include "scene/SceneManager.hpp"

class StressTest : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;
	void Events(SDL_Event event) override;

private:
	SDL_Texture* texture;
};