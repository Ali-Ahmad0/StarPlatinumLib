#pragma once
#include "scene/Scene.hpp"
#include "../entities/Player.hpp"

class OutdoorScene : public IScene
{
public:
	void Ready() override;
	void Update(double delta) override;

private:
	Player m_player{};
};

