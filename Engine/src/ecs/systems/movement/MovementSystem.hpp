#pragma once
#include "../BaseSystem.hpp"

struct MovementSystem : public BaseSystem
{
	void update(double delta) override;

	void onEntityAdded(EntityID e) override;
	void onEntityRemoved(EntityID e) override;

private:
	std::vector<EntityID> entities;
};