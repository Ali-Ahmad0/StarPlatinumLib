#include "TDPlayer.hpp"

#include "input/InputMap.hpp"
#include "main/debug/Debug.hpp"

void TDPlayer::Ready() 
{
	// Load player texture
	SDL_Texture* texture = TextureManager::LoadTexture("src/test/assets/topdown/character.png");

	// Create player entity
	m_entityId = ECS::CreateEntity();
		
	// Add components
	ECS::AddComponent(m_entityId, Transform(Vector2(32, 384), 0.0f, 3));
	ECS::AddComponent(m_entityId, Sprite(texture, 3, 4, 6, 0));

	ECS::AddComponent(m_entityId, Collider(Vector2(8, 20), 12.0f, 12.0f));
	ECS::AddComponent(m_entityId, Movement());
	
	m_transform = ECS::GetComponent<Transform>(m_entityId);

	// Configure sprite animations
	m_sprite = ECS::GetComponent<Sprite>(m_entityId);

	m_sprite->AddAnimation("idle_right", { 10 });
	m_sprite->AddAnimation("idle_left", { 7 });
	m_sprite->AddAnimation("idle_up", { 4 });
	m_sprite->AddAnimation("idle_down", { 1 });

	m_sprite->AddAnimation("walk_right", { 9, 10, 11 });
	m_sprite->AddAnimation("walk_left", { 6, 7, 8 });
	m_sprite->AddAnimation("walk_up", { 3, 4, 5 });
	m_sprite->AddAnimation("walk_down", { 0, 1, 2 });

	m_sprite->SetAnimation("idle_right");
	 
	m_movement = ECS::GetComponent<Movement>(m_entityId);

	// Set camera boundaries
	Camera::SetBoundaries(0, 960, 0, 720);
}

void TDPlayer::Update(double delta)
{
	// Get input direction using the InputMap system
	Vector2 input; 
	InputMap::GetVector("move_l", "move_r", "move_u", "move_d", input);

	// User input
	if (input.x != 0 || input.y != 0) {
		// Horizontal movement
		if (input.x < 0) {
			m_direction = Vector2(-1, 0);
			m_sprite->SetAnimation("walk_left");
		}
		else if (input.x > 0) {
			m_direction = Vector2(1, 0);
			m_sprite->SetAnimation("walk_right");
		}

		// Vertical movement
		else if (input.y < 0) {
			m_direction = Vector2(0, -1);
			m_sprite->SetAnimation("walk_up");
		}
		else if (input.y > 0) {
			m_direction = Vector2(0, 1);
			m_sprite->SetAnimation("walk_down");
		}

		// Set movement speed when there's input
		m_movement->speed.x = 128.0f;
		m_movement->speed.y = 128.0f;
	}
	else {
		// Set idle animation based on the last direction
		if (m_direction.x < 0) {
			m_sprite->SetAnimation("idle_left");
		}
		else if (m_direction.x > 0) {
			m_sprite->SetAnimation("idle_right");
		}
		else if (m_direction.y < 0) {
			m_sprite->SetAnimation("idle_up");
		}
		else if (m_direction.y > 0) {
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
}