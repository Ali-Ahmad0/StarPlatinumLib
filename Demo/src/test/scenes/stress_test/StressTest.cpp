#include "StressTest.hpp"

void StressTest::Ready() 
{
	texture = TextureManager::LoadTexture("src/test/assets/preview.png");
}

void StressTest::Events(SDL_Event event) 
{
	switch (event.key.keysym.sym)
	{
		// Stress test the engine by spawning a 1000 entites when enter is pressed
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
					float randomY = (float)(rand() % 480) - 32;

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