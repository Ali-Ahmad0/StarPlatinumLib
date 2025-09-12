#include "Player.hpp"

#include "input/InputMap.hpp"
#include "texture/TextureManager.hpp"

EntityID playerID;

void Player::Ready() 
{
	playerID = ECS::CreateEntity();
	
	// Add transform component
	const Vector2 position = Vector2(160.0f, 120.0f);
	const float rotation = 0.0f;
	const size_t scaling = 3;

	ECS::AddComponent(playerID, Transform(position, rotation, scaling));
	m_transform = ECS::GetComponent<Transform>(playerID);

	// Add sprite component
	SDL_Texture* texture = TextureManager::LoadTexture("src/assets/characters/player.png");
	const size_t hframes = 6;
	const size_t vframes = 10;
	const size_t animationSpeed = 12;
	const int8_t zIndex = 2;

	ECS::AddComponent(playerID, Sprite(texture, hframes, vframes, zIndex));
	m_sprite = ECS::GetComponent<Sprite>(playerID);

	// Add sprite animations
	m_sprite->AddAnimation("idle_down", { 0, 1, 2, 3, 4, 5 }, animationSpeed);
	m_sprite->AddAnimation("idle_side", { 6, 7, 8, 9, 10, 11 }, animationSpeed);
	m_sprite->AddAnimation("idle_up", { 12, 13, 14, 15, 16, 17 }, animationSpeed);

	m_sprite->AddAnimation("walk_down", { 18, 19, 20, 21, 22, 23 }, animationSpeed);
	m_sprite->AddAnimation("walk_side", { 24, 25, 26, 27, 28, 29 }, animationSpeed);
	m_sprite->AddAnimation("walk_up", { 30, 31, 32, 33, 34, 35 }, animationSpeed);

	// Add movement component
	ECS::AddComponent(playerID, Movement());
	m_movement = ECS::GetComponent<Movement>(playerID);

	// Add collider component
	const Vector2 centerOffset = Vector2(24.0f, 40.0f);
	const float colliderWidth = 10.0f;
	const float colliderHeight = 6.0f;

	ECS::AddComponent(playerID, Collider(centerOffset, colliderWidth, colliderHeight));

	// Set camera boundaries
	Camera::SetBoundaries(0, 960, 0, 720);
}

void Player::Update(double delta)
{
	// Get input direction using the InputMap
	Vector2 input;
	InputMap::GetVector("move_l", "move_r", "move_u", "move_d", input);

	// User input
	if (input.x != 0 || input.y != 0) 
	{
		// Horizontal movement
		if (input.x < 0) 
		{
			m_direction = Vector2(-1, 0);
			m_sprite->SetAnimation("walk_side");
			m_sprite->fliph = true;
		}
		else if (input.x > 0) 
		{
			m_direction = Vector2(1, 0);
			m_sprite->SetAnimation("walk_side");
			m_sprite->fliph = false;
		}

		// Vertical movement
		else if (input.y < 0) 
		{
			m_direction = Vector2(0, -1);
			m_sprite->SetAnimation("walk_up");
		}
		else if (input.y > 0) 
		{
			m_direction = Vector2(0, 1);
			m_sprite->SetAnimation("walk_down");
		}

		// Set movement speed when there's input
		m_movement->speed.x = 128.0f;
		m_movement->speed.y = 128.0f;
	}
	else 
	{
		// Set idle animation based on the last direction
		if (m_direction.x < 0) 
		{
			m_sprite->SetAnimation("idle_side");
			m_sprite->fliph = true;
		}
		else if (m_direction.x > 0) 
		{
			m_sprite->SetAnimation("idle_side");
			m_sprite->fliph = false;
		}
		else if (m_direction.y < 0) 
		{
			m_sprite->SetAnimation("idle_up");
		}
		else if (m_direction.y > 0) 
		{
			m_sprite->SetAnimation("idle_down");
		}

		// No movement when idle
		m_movement->speed.x = 0.0f;
		m_movement->speed.y = 0.0f;
	}

	// Set the movement direction from the input system
	m_movement->direction = input;

	// Update camera position
	Camera::SetOffset(Vector2(m_transform->position.x - 320, m_transform->position.y - 240));

	Collider* collider = ECS::GetComponent<Collider>(playerID);
}