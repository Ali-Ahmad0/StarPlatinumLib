#include "TestScene.hpp"
#include "../../../src/main/Game.hpp"

void TestScene::Ready()
{
	playerTexture = TextureManager::LoadTexture("res/test/assets/character.png");
	playerPreview = TextureManager::LoadTexture("res/test/assets/character_preview.png");

	// Create player entity
	player = Engine::GetECS().CreateEntity();
	player2 = Engine::GetECS().CreateEntity();

	// Add components
	Engine::GetECS().AddComponent(player, Transform(Vector2(0, 0), 4));
	Engine::GetECS().AddComponent(player, Sprite(playerTexture, 3, 4, 6));

	Engine::GetECS().AddComponent(player, Movement());

	Engine::GetECS().AddComponent(player, AABB(Vector2(8, 20), Vector2(12, 12), true));

	Engine::GetECS().AddComponent(player2, Transform(Vector2(272, 112), 4));
	Engine::GetECS().AddComponent(player2, Sprite(playerPreview));
	Engine::GetECS().AddComponent(player2, AABB(Vector2(8, 20), Vector2(12, 12), true));

	tilemap.AddTileset("res/test/assets/untitled.png");
	tilemap.LoadMap("res/test/assets/level/untitled.json");

	tilemap.AddCollision(1, { 28, 31, 37, 81, 82, 84, 89, 90 });
}

void TestScene::Update(double delta)
{
	//printf("Update...\n");
}

void TestScene::Events(SDL_Event event)
{
	auto sprite = Engine::GetECS().GetComponent<Sprite>(player);
	auto movement = Engine::GetECS().GetComponent<Movement>(player);

	movement->speed = 100;

	// Input and animation test
	switch (event.key.keysym.sym)
	{
	case SDLK_UP:
		sprite->v_frame = 1;
		movement->direction = Vector2(0, -1);
		break;

	case SDLK_DOWN:
		sprite->v_frame = 0;
		movement->direction = Vector2(0, 1);
		break;

	case SDLK_LEFT:
		sprite->v_frame = 2;
		movement->direction = Vector2(-1, 0);
		break;

	case SDLK_RIGHT:
		sprite->v_frame = 3;
		movement->direction = Vector2(1, 0);
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
	tilemap.DrawMap();
}