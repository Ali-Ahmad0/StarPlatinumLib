#include "SystemManager.hpp"

void SystemManager::OnEntityDestroyed(EntityID entity)
{
	for (auto const& pair : systems)
	{
		// Remove deleted entity from all systems
		auto const& system = pair.second;
		auto position = std::find(system->entities.begin(), system->entities.end(), entity);
		system->entities.erase(position);
		system->entityRecord[entity] = false;
	}
}

void SystemManager::OnEntitySignatureChanged(EntityID entity, ComponentSignature entitySignature)
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
				if (!system->entityRecord[entity]) 
				{
					system->entities.push_back(entity);
					system->entityRecord[entity] = true;
				}
			}

			// Otherwise
			else
			{
				// Remove entity from the system
				if (system->entityRecord[entity]) 
				{
					auto position = std::find(system->entities.begin(), system->entities.end(), entity);
					system->entities.erase(position);
					system->entityRecord[entity] = false;
				}
			}
		}
	}
}
