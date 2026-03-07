#pragma once
#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <iostream>
#include <vector>

#include "Entity.h"
#include "EntityManager.h"
#include "ComponenetStore.h"
#include "TransformComponent.h"
#include "TransformSystem.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};
class Game
{
public:
    // game state
    GameState    State;
    bool         Keys[1024];
    unsigned int Width, Height;

	EntityManager entityManager;
	ComponentStore<TransformComponent> transformStore;
    TransformSystem transformSystem = TransformSystem();

    static Game& getInstance(); 
    // constructor/destructor
    Game();
    ~Game();
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif
