#include "SystemManager.hpp"

void SystemManager::OnEntityDestroyed(EntityID entity)
{
	for (auto const& pair : m_systems)
	{
		// Remove deleted entity from all systems
		auto const& system = pair.second;
		system->entityRecord[entity] = false;
		system->OnEntityRemoved(entity);
	}
}

void SystemManager::OnEntitySignatureChanged(EntityID entity, Signature entitySignature)
{
	for (auto const& pair : m_systems)
	{
		auto const& type = pair.first;
		auto const& system = pair.second;

		// Get system signature
		auto const& systemSignature = m_signatures[type];

		// Check if enity signature matches system signature
		if ((entitySignature & systemSignature) == systemSignature)
		{
			// Add entity to the system
			if (!system->entityRecord[entity])
			{
				system->entityRecord[entity] = true;
				system->OnEntityAdded(entity);
			}
		}

		// Otherwise
		else
		{
			// Remove entity from the system
			if (system->entityRecord[entity])
			{
				system->entityRecord[entity] = false;
				system->OnEntityRemoved(entity);
			}
		}
	}
}
