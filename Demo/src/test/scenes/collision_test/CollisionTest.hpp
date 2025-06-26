#pragma once

#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/ecs/defintions.hpp"

class CollisionTest : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;

private:
	std::vector<EntityID> entities;
};

