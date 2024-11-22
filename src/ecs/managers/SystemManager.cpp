#include "SystemManager.hpp"

void SystemManager::OnEntityDestroyed(EntityID entity)
{
	for (auto const& pair : systems)
	{
		// Remove deleted entity from all systems
		auto const& system = pair.second;
		system->entities.erase(entity);
	}
}

void SystemManager::OnEntitySignatureChanged(EntityID entity, Signature entitySignature)
{
	{
		for (auto const& pair : systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;

			// Get system signature
			auto const& systemSignature = signatures[type];

			// Check if enity signature matches system signature
			if ((entitySignature & systemSignature) == systemSignature)
			{
				// Add entity to the system
				system->entities.insert(entity);
			}

			// Otherwise
			else
			{
				// Remove entity from the system
				system->entities.erase(entity);
			}
		}
	}
}
