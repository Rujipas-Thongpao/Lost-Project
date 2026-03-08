#include "game.h"

Game& Game::getInstance() {
		static Game instance;
		return instance;
}

Game::Game()
    : State(GAME_ACTIVE)
{

}

Game::~Game()
{

}

void Game::Init()
{
	transformSystem = TransformSystem();
	entityManager.Init();
	rendererSystem.Init();
	inputSystem.Init();

	Entity player = entityManager.CreateEntity();
	transformStore.add(player.id);
	//modelLoader.load(player, "Model/Maxwell/Untitled.obj", false);
	modelLoader.load(player, "Model/Manta/Manta.obj", false);

	Entity camera = entityManager.CreateEntity();
	cameraStore.add(camera.id);
	transformStore.add(camera.id);
}

void Game::Update(float dt)
{
	transformSystem.Update();	
	inputSystem.Update(dt);
	rendererSystem.Render();
}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{

}
