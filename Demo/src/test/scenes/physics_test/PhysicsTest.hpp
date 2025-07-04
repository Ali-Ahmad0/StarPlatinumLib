#pragma once

#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/ecs/definitions.hpp"

class PhysicsTest : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;

private:
	std::vector<EntityID> entities;
};

