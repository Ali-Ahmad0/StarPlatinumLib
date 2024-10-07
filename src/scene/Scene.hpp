#pragma once
#pragma once
#include <SDL.h>

// All scenes will extend this
class IScene 
{
public:
	virtual ~IScene() = default;
	virtual void Ready() {}
	virtual void Update(double delta) {}
	virtual void Events(SDL_Event event) {}
	virtual void Draw() {}
};