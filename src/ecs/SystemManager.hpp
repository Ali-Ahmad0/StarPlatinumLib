#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include "systems/BaseSystem.hpp"

class SystemManager 
{
public:

	template <typename T>
	std::shared_ptr<T> RegisterSystem() 
	{
		std::type_index typeIndex = typeid(T);
		if (!isRegistered(typeIndex)) 
		{
			systems[typeIndex] = std::make_shared<T>();
			return system;
		}
		throw std::runtime_error("System is already registered\n");
	}

	template <typename T>
	void SetSignatrue(Signature signature) 
	{
		std::type_index typeIndex = typeid(T);
		if (isRegistered(typeIndex)) 
		{
			signatures[typeIndex] = signature;
		}

		else 
		{
			fprintf(stderr, "Cannot set signature on unregistered system");
		}
	}

	void OnEntityDestroyed(EntityID entity) 
	{
		for (auto const& pair : systems) 
		{
			auto const& system = pair.second;
			system->entities.erase(entity);
		}
	}

	void EntitySignatureChanged(EntityID entity, Signature entitySignature) 
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

private:
	std::unordered_map<std::type_index, Signature> signatures{};

	// Map from type of system to the system
	std::unordered_map<std::type_index, std::shared_ptr<BaseSystem>> systems{};

	bool isRegistered(std::type_index typeIndex) 
	{
		return systems.find(typeIndex) != systems.end();
	}
};

#endif // !SYSTEM_MANAGER_HPP
