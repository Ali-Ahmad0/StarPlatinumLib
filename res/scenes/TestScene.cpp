#include "TestScene.hpp"
#include "../../src/main/Game.hpp"

void TestScene::Ready() 
{
	playerTexture = TextureManager::LoadTexture("assets/character.png");
	playerPreview = TextureManager::LoadTexture("assets/character_preview.png");

	// Create player entity
	player = Engine::GetECS().CreateEntity();

	// Add components
	Engine::GetECS().AddComponent(player, Transform(Vector2(288, 172), 4));
	Engine::GetECS().AddComponent(player, Sprite(playerTexture, 3, 4, 6));
	Engine::GetECS().AddComponent(player, AABB(Vector2(16, 16)));

	tilemap = Tilemap("assets/tileset.png", 8, 13);
	tilemap.LoadMap("assets/level/tilemap.json");
}

void TestScene::Update(double delta) 
{
	//printf("Update...\n");

	std::cout 
		<< "X Boundary: (" << Engine::GetECS().GetComponent<AABB>(player)->min.x << ", " << Engine::GetECS().GetComponent<AABB>(player)->max.x << ")\n"
		<< "Y Boundary: (" << Engine::GetECS().GetComponent<AABB>(player)->min.y << ", " << Engine::GetECS().GetComponent<AABB>(player)->max.y << ")\n";

	std::cout << "Collision: " << Engine::GetECS().GetComponent<AABB>(player)->colliding() << "\n";
}

void TestScene::Draw() 
{
	tilemap.DrawMap(4);
}