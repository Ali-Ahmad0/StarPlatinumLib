#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include "../systems/BaseSystem.hpp"

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
			return std::make_shared<T>();
		}
		throw std::runtime_error("System is already registered\n");
	}

	template <typename T>
	std::shared_ptr<T> GetSystem()
	{
		std::type_index typeIndex = typeid(T);
		if (isRegistered(typeIndex))
		{
			return std::static_pointer_cast<T>(systems[typeIndex]);
		}
		throw std::runtime_error("Cannot get unregistered system\n");
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

	void OnEntityDestroyed(EntityID entity);
	void OnEntitySignatureChanged(EntityID entity, Signature entitySignature);

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
