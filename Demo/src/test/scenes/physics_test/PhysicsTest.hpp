#pragma once

#include "scene/SceneManager.hpp"
#include "ecs/definitions.hpp"

class PhysicsTest : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;

private:
	std::vector<EntityID> entities;
};

