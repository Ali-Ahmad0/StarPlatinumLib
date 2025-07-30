#pragma once
#include "scene/Scene.hpp"

class PhysicsTest : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;
	void Events(SDL_Event event) override;
};

