#pragma once
#include "ecs/ECS.hpp"

class Player
{
public:
	void Ready();
	void Update(double delta);

private:
	EntityID m_id;

	Transform* m_transform = nullptr;
	Sprite* m_sprite = nullptr;
	Movement* m_movement = nullptr;

	Vector2 m_direction = Vector2::ZERO;
};

