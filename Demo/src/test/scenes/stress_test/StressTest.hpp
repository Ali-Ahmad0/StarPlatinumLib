#pragma once
#include "scene/Scene.hpp"

class StressTest : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;
	void Events(SDL_Event event) override;

private:
	SDL_Texture* m_texture;
};