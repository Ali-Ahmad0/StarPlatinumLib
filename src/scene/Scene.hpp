#pragma once
#pragma once

// All scenes will extend this
class IScene 
{
public:
	virtual ~IScene() = default;
	virtual void Ready() = 0;
	virtual void Update(double delta) = 0;
	virtual void Draw() = 0;
};