#pragma once

#include "../systems/BaseSystem.hpp"

class SystemManager 
{
public:

	// Register a new system
	template <typename T>
	std::shared_ptr<T> RegisterSystem() 
	{
		const std::type_index typeIndex = typeid(T);
		if (!isRegistered(typeIndex)) 
		{
			auto system = std::make_shared<T>();
			systems[typeIndex] = system;
			printf("[INFO]: Registered system: %s\n", typeIndex.name());
			return system;
		}
		throw std::runtime_error("[RUNTIME ERROR]: System is already registered");
	}

	// Return pointer to a system
	template <typename T>
	std::shared_ptr<T> GetSystem()
	{
		const std::type_index typeIndex = typeid(T);
		if (isRegistered(typeIndex))
		{
			return std::static_pointer_cast<T>(systems[typeIndex]);
		}
		throw std::runtime_error("[RUNTIME ERROR]: Cannot get unregistered system");
	}

	template <typename T>
	ComponentSignature GetSignature()
	{
		const std::type_index typeIndex = typeid(T);
		if (isRegistered(typeIndex))
		{
			return signatures[typeIndex];
		}

		throw std::runtime_error("[RUNTIME ERROR]: Cannot get signature on unregistered system");
	}
	
	template <typename T>
	void SetSignature(ComponentSignature signature)
	{
		const std::type_index typeIndex = typeid(T);
		if (isRegistered(typeIndex)) 
		{
			signatures[typeIndex] = signature;
		}

		else 
		{
			fprintf(stderr, "[ERROR]: Cannot set signature on unregistered system");
		}
	}

	// Notify all systems that an entity has been destroyed
	void OnEntityDestroyed(EntityID entity);

	// Notify all systems that entity signature has changed
	void OnEntitySignatureChanged(EntityID entity, ComponentSignature entitySignature);

private:
	// Map from type of system to signature
	std::unordered_map<std::type_index, ComponentSignature> signatures{};

	// Map from type of system to the system
	std::unordered_map<std::type_index, std::shared_ptr<BaseSystem>> systems{};

	// Returns if system has been registered or not
	bool isRegistered(std::type_index typeIndex) 
	{
		return systems.find(typeIndex) != systems.end();
	}
};

