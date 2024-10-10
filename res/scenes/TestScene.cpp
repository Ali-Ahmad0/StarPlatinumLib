#include "TestScene.hpp"
#include "../../src/main/Game.hpp"

void TestScene::Ready()
{
	playerTexture = TextureManager::LoadTexture("res/assets/character.png");
	playerPreview = TextureManager::LoadTexture("res/assets/character_preview.png");

	// Create player entity
	player = Engine::GetECS().CreateEntity();

	// Add components
	Engine::GetECS().AddComponent(player, Transform(Vector2(288, 172), 4));
	Engine::GetECS().AddComponent(player, Sprite(playerTexture, 3, 4, 6));

	Engine::GetECS().AddComponent(player, AABB(Vector2(16, 16)));

	tilemap = Tilemap("res/assets/untitled.png");
	tilemap.LoadMap("res/assets/level/untitled.json");
}

void TestScene::Update(double delta)
{
	//printf("Update...\n");

	//std::cout 
	//	<< "X Boundary: (" << Engine::GetECS().GetComponent<AABB>(player)->min.x << ", " << Engine::GetECS().GetComponent<AABB>(player)->max.x << ")\n"
	//	<< "Y Boundary: (" << Engine::GetECS().GetComponent<AABB>(player)->min.y << ", " << Engine::GetECS().GetComponent<AABB>(player)->max.y << ")\n";

	//std::cout << "Collision: " << Engine::GetECS().GetComponent<AABB>(player)->colliding() << "\n";
}


void TestScene::Events(SDL_Event event)
{
	auto sprite = Engine::GetECS().GetComponent<Sprite>(player);

	// Input and animation test
	switch (event.key.keysym.sym)
	{
	case SDLK_UP:
		sprite->v_frame = 1;
		break;

	case SDLK_DOWN:
		sprite->v_frame = 0;
		break;

	case SDLK_LEFT:
		sprite->v_frame = 2;
		break;

	case SDLK_RIGHT:
		sprite->v_frame = 3;
		break;

	// Stress test
	case SDLK_RETURN:

		// Spawn entities in random positions
		srand((unsigned int)(time(nullptr)));
		try
		{
			for (int i = 0; i < 100; i++)
			{
				EntityID entity = Engine::GetECS().CreateEntity();

				// Set random positions within the screen bounds (640 x 480)
				float randomX = (float)(rand() % 640);
				float randomY = (float)(rand() % 480) - 32;

				// Assign components to the entity
				Engine::GetECS().AddComponent(entity, Transform(Vector2(randomX, randomY), 3));
				Engine::GetECS().AddComponent(entity, Sprite(playerPreview));

			}
		}

		catch (const std::runtime_error& e)
		{
			printf("%s\n", e.what());
		}

	default:
		break;
	}
}

void TestScene::Draw() 
{
	tilemap.DrawMap(4);
}