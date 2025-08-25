#pragma once
#include "../ISystem.hpp"

struct MovementSystem : public ISystem
{
	void Update(double delta) override;

	void OnEntityAdded(EntityID e) override;
	void OnEntityRemoved(EntityID e) override;
	void OnAllEntitiesRemoved() override;

private:
	std::vector<EntityID> m_entities;
};