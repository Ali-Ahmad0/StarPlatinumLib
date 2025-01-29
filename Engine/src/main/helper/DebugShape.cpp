#include "../Game.hpp"
#include "DebugShape.hpp"

// Draw a rectangle
void DebugShape::DrawRect(const Vector2& position, float w, float h, const Color& color) 
{
	// Set draw color
	SDL_SetRenderDrawColor(Engine::GetRenderer(), color.r, color.g, color.b, color.a);

	// Draw rect
	SDL_FRect rect = { position.x, position.y, w, h };
	SDL_RenderDrawRectF(Engine::GetRenderer(), &rect);

	// Reset draw color
	SDL_SetRenderDrawColor(Engine::GetRenderer(), 255, 255, 255, 255);
}




