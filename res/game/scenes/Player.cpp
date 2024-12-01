#include "Player.hpp"

void Player::Ready() 
{
	playerTexture = TextureManager::LoadTexture("res/game/assets/player/player.png");
	player = ECS::CreateEntity();

	ECS::AddComponent<Transform>(player, Transform(Vector2(64, 200), 0, 4));
	ECS::AddComponent<Sprite>(player, Sprite(playerTexture, 7, 4, 8));
	ECS::AddComponent<AABB>(player, AABB(Vector2(8, 8), Vector2(16, 16)));
   
	ECS::AddComponent<Movement>(player, Movement());

	movement = ECS::GetComponent<Movement>(player);
	sprite = ECS::GetComponent<Sprite>(player);
}

void Player::Update(double delta) 
{
    const Uint8* state = SDL_GetKeyboardState(NULL);

    Vector2 newDirection(0, 1);

    // Left right movement
    if (state[SDL_SCANCODE_LEFT])
    {
        newDirection.x = -1;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
        newDirection.x = 1;
    }

    movement->speed.x = 250;
    movement->speed.y = 500;

    movement->direction = newDirection;
}