#pragma once
#include "../ECS.hpp"
#include "BaseSystem.hpp"

struct GravitySystem : public BaseSystem 
{
	void update(double delta);
};