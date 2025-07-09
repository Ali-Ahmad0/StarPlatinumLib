#include "TDPlayer.hpp"

#include "input/InputMap.hpp"
#include "main/debug/Debug.hpp"

void TDPlayer::Ready() 
{
	// Load player texture
	texture = TextureManager::LoadTexture("src/test/assets/topdown/character.png");

	// Create player entity
	player = ECS::CreateEntity();
		
	// Add components
	ECS::AddComponent(player, Transform(Vector2(32, 384), 0.0f, 3));
	ECS::AddComponent(player, Sprite(texture, 3, 4, 6, 0));

	ECS::AddComponent(player, Collider(Vector2(8, 20), 12.0f, 12.0f));
	ECS::AddComponent(player, Movement());
	
	transform = ECS::GetComponent<Transform>(player);

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

	sprite->setAnim("idle_right");
	 
	movement = ECS::GetComponent<Movement>(player);

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
			direction = Vector2(-1, 0);
			sprite->setAnim("walk_left");
		}
		else if (input.x > 0) {
			direction = Vector2(1, 0);
			sprite->setAnim("walk_right");
		}

		// Vertical movement
		else if (input.y < 0) {
			direction = Vector2(0, -1);
			sprite->setAnim("walk_up");
		}
		else if (input.y > 0) {
			direction = Vector2(0, 1);
			sprite->setAnim("walk_down");
		}

		// Set movement speed when there's input
		movement->speed.x = 128.0f;
		movement->speed.y = 128.0f;
	}
	else {
		// Set idle animation based on the last direction
		if (direction.x < 0) {
			sprite->setAnim("idle_left");
		}
		else if (direction.x > 0) {
			sprite->setAnim("idle_right");
		}
		else if (direction.y < 0) {
			sprite->setAnim("idle_up");
		}
		else if (direction.y > 0) {
			sprite->setAnim("idle_down");
		}

		// No movement when idle
		movement->speed.x = 0.0f;
		movement->speed.y = 0.0f;
	}

	// Set the movement direction from the input system
	movement->direction = input;

	// Update camera position
	Camera::SetOffset(Vector2(transform->position.x - 320, transform->position.y - 240));
}