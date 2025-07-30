#include "VIntegrationSystem.hpp"

void VIntegrationSystem::update(double delta)
{

}

void VIntegrationSystem::onEntityAdded(EntityID e)
{
	// Add the entity
	entities.push_back(e);
}

void VIntegrationSystem::onEntityRemoved(EntityID e)
{
	// Find and delete entity
	auto position = std::find(entities.begin(), entities.end(), e);
	entities.erase(position);
}
