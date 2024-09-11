#ifndef BASE_SYSTEM_HPP
#define BASE_SYSTEM_HPP
#include "../defintions.hpp"

// Base struct for all systems with a set of entities
struct BaseSystem 
{
	std::set<EntityID> entities;
};

#endif // !BASE_SYSTEM_HPP
