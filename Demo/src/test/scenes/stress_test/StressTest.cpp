#include "StressTest.hpp"
#include "texture/TextureManager.hpp"
#include "text/TextManager.hpp"

void StressTest::Ready() 
{
	texture = TextureManager::LoadTexture("src/test/assets/preview.png");
	TextManager::LoadFont("default", "src/test/assets/font.ttf");
}

void StressTest::Update(double delta)
{
	// Update FPS only once per second
	static double fpsTimer = 0.0;
	static int cachedFPS = 0;
	static std::string cachedFPSText = "FPS: 0";

	fpsTimer += delta;

	// Update every second
	if (fpsTimer >= 1.0)
	{
		// Calculate FPS
		double targetDelta = 1000 / 60;
		cachedFPS = (int)(targetDelta / (delta * 1000) * 60);

		// Format FPS string
		std::ostringstream fpsStream;
		fpsStream << "FPS: " << cachedFPS;
		cachedFPSText = fpsStream.str();

		fpsTimer = 0.0;
	}

	// Draw the FPS text
	TextManager::DrawText(cachedFPSText.c_str(), "default", Vector2(16, 16));

	// Get entity count
	std::ostringstream countStream;
	countStream << "Entities: " << ECS::GetEntityCount();
	std::string countText = countStream.str();

	// Draw entity count
	TextManager::DrawText(countText.c_str(), "default", Vector2(500, 16));
}

void StressTest::Events(SDL_Event event)
{
	if (event.type != SDL_KEYDOWN) 
	{
		return;
	}

	switch (event.key.keysym.sym)
	{
		// Stress test the engine by spawning a lot of entites when enter is pressed
		case SDLK_RETURN:

			// Spawn entities in random positions
			srand((unsigned int)(time(nullptr)));
			try
			{
				for (int i = 0; i < 1000; i++)
				{
					EntityID entity = ECS::CreateEntity();

					// Set random positions within the screen bounds (640 x 480)
					float randomX = (float)(rand() % 640);
					float randomY = (float)(rand() % 480) + 32;

					// Assign components to the entity
					ECS::AddComponent(entity, Transform(Vector2(randomX, randomY), 0, 3));
					ECS::AddComponent(entity, Sprite(texture));
				}
			}

			catch (const std::runtime_error& e)
			{
				fprintf(stderr, "[ERROR]: %s\n", e.what());
			}

			break;
		default: 
			break;
	}
}