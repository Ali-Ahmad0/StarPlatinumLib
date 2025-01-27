#pragma once
#include "../BaseSystem.hpp"

struct MovementSystem : public BaseSystem
{
	std::vector<EntityID> entities;

	void update(double delta);

	void onEntityAdded(EntityID e) override;
	void onEntityRemoved(EntityID e) override;
};