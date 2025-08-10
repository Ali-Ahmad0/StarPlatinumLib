#pragma once
#include "texture/TextureManager.hpp"

class TDPlayer
{
public:
	void Ready();
	void Update(double delta);
	
private:
	EntityID m_entityId;

	Vector2 m_direction;

	Transform* m_transform;
	Sprite* m_sprite;
	Movement* m_movement;
};