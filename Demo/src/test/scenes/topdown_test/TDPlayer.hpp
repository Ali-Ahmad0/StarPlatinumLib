#pragma once
#include "texture/TextureManager.hpp"
#include "scene/Scene.hpp"

class TDPlayer : IScene
{
public:
	void Ready() override;
	void Update(double delta) override;
	
private:
	Vector2 m_direction;

	Transform* m_transform;
	Sprite* m_sprite;
	Movement* m_movement;
};