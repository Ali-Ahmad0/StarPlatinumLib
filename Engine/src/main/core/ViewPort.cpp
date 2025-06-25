#include "ViewPort.hpp"

SDL_Window* ViewPort::window = nullptr;
SDL_Renderer* ViewPort::renderer = nullptr;

bool ViewPort::Init(const char* title, int w, int h, bool fullscreen, const Vector2& position)
{
	int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		printf("[INFO]: Initialized subsystems\n");

		// Create window
		window = SDL_CreateWindow(
			title, (int)position.x, (int)position.y, w, h, flags
		);

		if (window == nullptr)
		{
			fprintf(stderr, "[ERROR]: Unable to create SDL window, exiting...\n");
			return false;
		}

		printf("[INFO]: Window created\n");


		// Create renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr)
		{
			fprintf(stderr, "[ERROR]: Unable to create SDL renderer, exiting...\n");
			return false;
		}

		printf("[INFO]: Renderer created\n");
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}

	else
	{
		fprintf(stderr, "[ERROR] Failed to initialize SDL, exiting...\n");
		return false;
	}

	return true;
}

void ViewPort::Exit() 
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

SDL_Window* ViewPort::GetWindow() { return window; }
SDL_Renderer* ViewPort::GetRenderer() { return renderer; }