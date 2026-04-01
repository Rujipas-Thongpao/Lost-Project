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
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "ModelLoader.h"
#include "RendererSystem.h"
#include "CameraComponent.h"
#include "InputSystem.h"
#include "LightComponent.h"
#include "TagStore.h"
#include "CameraSystem.h"
#include "ColliderComponent.h"
#include "ColliderSystem.h"
#include "BulletComponent.h"
#include "GunSystem.h"
#include "AnimationComponent.h"
#include "AnimationSystem.h"
#include "Asset.h"
#include "SpriteComponent.h"
#include "ParticleComponent.h"
#include "ParticleSystem.h"

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
    unsigned int Width, Height;

	EntityManager entityManager;
    Assets assetManager;
	ComponentStore<TransformComponent> transformStore;
    TransformSystem transformSystem = TransformSystem();
	ComponentStore<MeshComponent> meshStore;
	ComponentStore<MaterialComponent> materialStore;
    ComponentStore<CameraComponent> cameraStore;
    ComponentStore<LightComponent> lightStore;
    ComponentStore<ColliderComponent> colliderStore;
    ComponentStore<BulletComponent> bulletStore;
    ComponentStore<AnimationComponent> animationStore;
    ComponentStore<SpriteComponent> spriteStore;
	ComponentStore<ParticleComponent> particleStore;

    ModelLoader modelLoader;

	RendererSystem rendererSystem;
    InputSystem inputSystem;
    TagStore tagStore;
    CameraSystem cameraSystem;
    ColliderSystem colliderSystem;
    GunSystem gunSystem;
    AnimationSystem animationSystem;
	ParticleSystem particleSystem;
    
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
