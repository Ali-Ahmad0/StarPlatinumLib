#pragma once
#include "../BaseSystem.hpp"

struct MovementSystem : public BaseSystem
{
	void Update(double delta) override;

	void OnEntityAdded(EntityID e) override;
	void OnEntityRemoved(EntityID e) override;

private:
	std::vector<EntityID> m_entities;
};