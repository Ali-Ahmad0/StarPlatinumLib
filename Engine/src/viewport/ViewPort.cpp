#include "ViewPort.hpp"

SDL_Window* ViewPort::s_window = nullptr;
SDL_Renderer* ViewPort::s_renderer = nullptr;

Color ViewPort::modulate = Color::WHITE;

bool ViewPort::Init(const char* title, int w, int h, bool fullscreen)
{
	int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		printf("[INFO]: Initialized subsystems\n");

		// Create window
		s_window = SDL_CreateWindow(
			title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags
		);

		if (s_window == nullptr)
		{
			fprintf(stderr, "[ERROR]: Unable to create SDL window, exiting...\n");
			return false;
		}

		printf("[INFO]: Window created\n");


		// Create renderer
		s_renderer = SDL_CreateRenderer(s_window, -1, SDL_RENDERER_ACCELERATED);
		if (s_renderer == nullptr)
		{
			fprintf(stderr, "[ERROR]: Unable to create SDL renderer, exiting...\n");
			return false;
		}

		printf("[INFO]: Renderer created\n");
		SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 255);
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
	printf("[INFO]: Destroying window and renderer\n");
	SDL_DestroyWindow(s_window);
	SDL_DestroyRenderer(s_renderer);
}

SDL_Window* ViewPort::GetWindow() 
{ 
	return s_window; 
}

SDL_Renderer* ViewPort::GetRenderer() 
{ 
	return s_renderer; 
}

void ViewPort::GetSize(int* w, int* h)
{
	SDL_GetWindowSize(s_window, w, h);
}