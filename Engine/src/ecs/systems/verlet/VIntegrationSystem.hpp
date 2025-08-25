#pragma once
#include "../ISystem.hpp"

struct VIntegrationSystem : ISystem
{
	void Update(double delta) override;

	void OnEntityAdded(EntityID e) override;
	void OnEntityRemoved(EntityID e) override;
	void OnAllEntitiesRemoved() override;

private:
	std::vector<EntityID> m_entities;
};