#pragma once

#include "../../../src/scene/SceneManager.hpp"
#include "../../../src/ecs/defintions.hpp"

class CollisionTest : public IScene
{
public:
	void Ready();
	void Update(double delta);

private:
	std::vector<EntityID> entities;
};

