#pragma once
#include "../ECS.hpp"
#include "BaseSystem.hpp"

#include <iostream>

struct MovementSystem : public BaseSystem
{
	void update(double delta);
};