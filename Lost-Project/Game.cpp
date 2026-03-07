#include "game.h"

Game& Game::getInstance() {
		static Game instance;
		return instance;
}

Game::Game()
    : State(GAME_ACTIVE), Keys()
{

}

Game::~Game()
{

}

void Game::Init()
{
	transformSystem = TransformSystem();
	entityManager.Init();
	Entity entity = entityManager.CreateEntity();
	transformStore.add(entity.id);
}

void Game::Update(float dt)
{
	transformSystem.Update();	
}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{

}
