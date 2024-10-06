#pragma once
#include "SceneManager.hpp"

class InitialScene : public IScene
{
public:

	void Ready() override;
	void Update(double delta) override;
	void Draw() override;
};