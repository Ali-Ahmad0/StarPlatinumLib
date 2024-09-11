#include "SystemManager.hpp"

void SystemManager::OnEntityDestroyed(const EntityID entity) const
{
	for (auto const& pair : systems)
	{
		auto const& system = pair.second;
		system->entities.erase(entity);
	}
}

void SystemManager::OnEntitySignatureChanged(const EntityID entity, const Signature entitySignature)
{
	{
		for (auto const& pair : systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;

			auto const& systemSignature = signatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->entities.insert(entity);
			}
			else
			{
				system->entities.erase(entity);
			}
		}
	}
}
