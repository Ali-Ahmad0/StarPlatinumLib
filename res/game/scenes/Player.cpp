#include "Player.hpp"

void Player::Ready() 
{
	playerTexture = TextureManager::LoadTexture("res/game/assets/player/player.png");
	player = ECS::CreateEntity();

	ECS::AddComponent<Transform>(player, Transform(Vector2(64, 188), 0, 4));
	ECS::AddComponent<Sprite>(player, Sprite(playerTexture, 7, 4, 8));
	ECS::AddComponent<AABB>(player, AABB(Vector2(8, 8), Vector2(16, 16)));
	ECS::AddComponent<Movement>(player, Movement());

	movement = ECS::GetComponent<Movement>(player);
	sprite = ECS::GetComponent<Sprite>(player);
}

void Player::Update(double delta) 
{
    const Uint8* state = SDL_GetKeyboardState(NULL);

    Vector2 newDirection(0, 0);
    if (state[SDL_SCANCODE_LEFT])
    {
        newDirection.x = -1;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        newDirection.x = 1;
    }

    movement->direction = newDirection;
    movement->speed = (newDirection.x != 0) ? 250.0f : 0.0f;
}

void Player::Events(SDL_Event event) 
{

}

void Player::Draw() 
{

}