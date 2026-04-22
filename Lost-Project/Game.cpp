#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include "game.h"
#include "MaterialComponent.h"
#include "RendererSystem.h"
#include <cstdlib>
#include "Asset.h"
#include "animation.h"
#include "ResourceManager.h"
#include "SpriteComponent.h"

Game& Game::getInstance() {
		static Game instance;
		return instance;
}

Game::Game()
{

}

Game::~Game()
{
	audioSystem.Shutdown();
}

void Game::Init()
{
	transformSystem = TransformSystem();
	entityManager.Init();
	rendererSystem.Init();
	inputSystem.Init();
	colliderSystem.Init();

	// load mesh
	//assetManager = Assets::getInstance();
	ResourceManager::LoadTexture("Texture/Circle.png",1.0, "Circle");

	assetManager.registerMesh("player_mesh", modelLoader.load("Model/Maxwell_2.fbx"));
	assetManager.registerMesh("cat_mesh", modelLoader.load("Model/cat-box.fbx"));
	assetManager.registerMesh("floor_mesh", modelLoader.load("Model/terrain.fbx"));
	assetManager.registerMesh("bullet_mesh", modelLoader.load("Model/Bullet/Bullet.obj"));
	assetManager.registerMesh("Quad", modelLoader.load("Model/Quad.fbx"));
	assetManager.registerMesh("Block", modelLoader.load("Model/Block.fbx"));
	assetManager.registerMesh("Tree_1", modelLoader.load("Model/Tree.fbx"));
	assetManager.registerMesh("Tree_2", modelLoader.load("Model/Tree_2.fbx"));

	assetManager.registerAnimation("player_idle",
		new Animation("Model/Maxwell_2_idle.fbx",
			modelLoader.modelDatas[assetManager.GetModelData("player_mesh")])
	);

	assetManager.registerAnimation("player_walk",
		new Animation("Model/Maxwell_2.fbx",
			modelLoader.modelDatas[assetManager.GetModelData("player_mesh")])
	);

	audioSystem.Init();
	audioSystem.Load("bgm",         "Sound/bgm.ogg");
	audioSystem.Load("shoot",       "Sound/Shoot.wav");
	audioSystem.Load("died",  "Sound/enemyDied.wav");
	audioSystem.Load("hurt", "Sound/playerhurt.wav");
	audioSystem.SetMasterVolume(0.5f);
	SoundConfig bgmCfg;
	bgmCfg.isLoop = true;
	bgmCfg.volume = 0.5f;
	audioSystem.Play("bgm", bgmCfg);

	world.Init();
	enemySystem.Init();
	gunSystem.Init();
	waveSystem.Init();
	healthSystem.Init();
	cameraSystem.Init();
	gui.Init();
}

void Game::Update(float dt)
{
	transformSystem.Update();	
	cameraSystem.Update(dt);
	colliderSystem.Update();

	gunSystem.Update(dt);

	enemySystem.Update(dt);
	waveSystem.Update(dt);
	statSystem.Update(dt);

	healthSystem.Update(dt);
	animationSystem.Update(dt);
	particleSystem.Update(dt);
	rendererSystem.Render();
	rendererSystem.RenderParticles();

	gui.Update(dt);
}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{

}
