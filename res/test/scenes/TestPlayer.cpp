#include "TestPlayer.hpp"

void TestPlayer::Ready() 
{
	// Create player entity
	player = ECS::CreateEntity();

	// Load player texture
	texture = TextureManager::LoadTexture("res/test/assets/character.png");

	// Add components
	ECS::AddComponent(player, Transform(Vector2(0, 0), 0.0, 8));
	ECS::AddComponent(player, Sprite(texture, 3, 4, 6));
	ECS::AddComponent(player, AABB(Vector2(8, 20), Vector2(12, 12)));
	ECS::AddComponent(player, Movement());

	// Configure sprite animations
	sprite = ECS::GetComponent<Sprite>(player);

	sprite->addAnim("idle_right", { 10 });
	sprite->addAnim("idle_left", { 7 });
	sprite->addAnim("idle_up", { 4 });
	sprite->addAnim("idle_down", { 1 });

	sprite->addAnim("walk_right", { 9, 10, 11 });
	sprite->addAnim("walk_left", { 6, 7, 8 });
	sprite->addAnim("walk_up", { 3, 4, 5 });
	sprite->addAnim("walk_down", { 0, 1, 2 });
 
	movement = ECS::GetComponent<Movement>(player);
}

void TestPlayer::Update(double delta) 
{
	// Basic input
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_LEFT])
	{
		direction = Vector2(-1, 0);
		sprite->setAnim("walk_left");
	}
	else if (state[SDL_SCANCODE_RIGHT])
	{
		direction = Vector2(1, 0);
		sprite->setAnim("walk_right");
	}
	else if (state[SDL_SCANCODE_UP])
	{
		direction = Vector2(0, -1);
		sprite->setAnim("walk_up");
	}
	else if (state[SDL_SCANCODE_DOWN])
	{
		direction = Vector2(0, 1);
		sprite->setAnim("walk_down");
	}
	else
	{
		// Set idle animation based on the last direction
		if (direction.x == -1)
		{
			sprite->setAnim("idle_left");
		}
		else if (direction.x == 1)
		{
			sprite->setAnim("idle_right");
		}
		else if (direction.y == -1)
		{
			sprite->setAnim("idle_up");
		}
		else if (direction.y == 1)
		{
			sprite->setAnim("idle_down");
		}
	}

	movement->direction = direction;
	movement->speed.x = 200.0f * (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT]
								|| state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_DOWN]);

	movement->speed.y = movement->speed.x;
}

void TestPlayer::Events(SDL_Event event) 
{
	// Stress test example
	//switch (event.key.keysym.sym)
	//{
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

	//default:
	//	break;
	//}
}