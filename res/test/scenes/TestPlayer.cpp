#include "TestPlayer.hpp"

void TestPlayer::Ready() 
{
	// Create player entity
	player = Engine::GetECS().CreateEntity();

	// Load player texture
	texture = TextureManager::LoadTexture("res/test/assets/character.png");

	// Add components
	Engine::GetECS().AddComponent(player, Transform(Vector2(0, 0), 0.0, 4));
	Engine::GetECS().AddComponent(player, Sprite(texture, 3, 4, 6));
	Engine::GetECS().AddComponent(player, AABB(Vector2(8, 16), Vector2(12, 12), true));
	Engine::GetECS().AddComponent(player, Movement());

	// Configure sprite animations
	sprite = Engine::GetECS().GetComponent<Sprite>(player);
	sprite->addAnim("walk_right", { 9, 10, 11 });
	sprite->addAnim("walk_left", { 6, 7, 8 });
	sprite->addAnim("walk_up", { 3, 4, 5 });
	sprite->addAnim("walk_down", { 0, 1, 2 });
 
	movement = Engine::GetECS().GetComponent<Movement>(player);
	movement->speed = 100;
}

void TestPlayer::Events(SDL_Event event) 
{
	// Input and animation test
	switch (event.key.keysym.sym)
	{
	case SDLK_UP:
		movement->direction = Vector2(0, -1);
		sprite->setAnim("walk_up");
		break;

	case SDLK_DOWN:
		movement->direction = Vector2(0, 1);
		sprite->setAnim("walk_down");
		break;

	case SDLK_LEFT:
		movement->direction = Vector2(-1, 0);
		sprite->setAnim("walk_left");
		break;

	case SDLK_RIGHT:
		movement->direction = Vector2(1, 0);
		sprite->setAnim("walk_right");
		break;

	//// Stress test
	//case SDLK_RETURN:

	//	// Spawn entities in random positions
	//	srand((unsigned int)(time(nullptr)));
	//	try
	//	{
	//		for (int i = 0; i < 100; i++)
	//		{
	//			EntityID entity = Engine::GetECS().CreateEntity();

	//			// Set random positions within the screen bounds (640 x 480)
	//			float randomX = (float)(rand() % 640);
	//			float randomY = (float)(rand() % 480) - 32;

	//			// Assign components to the entity
	//			Engine::GetECS().AddComponent(entity, Transform(Vector2(randomX, randomY), 3));
	//			Engine::GetECS().AddComponent(entity, Sprite(playerPreview));
	//		}
	//	}


	//	catch (const std::runtime_error& e)
	//	{
	//		printf("%s\n", e.what());
	//	}

	default:
		break;
	}
}