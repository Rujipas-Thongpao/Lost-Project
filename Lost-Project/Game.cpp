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
	colliderSystem.Init();
	gunSystem.Init();
	enemySystem.Init();

	// load mesh
	//assetManager = Assets::getInstance();
	ResourceManager::LoadTexture("Texture/Circle.png",1.0, "Circle");

	assetManager.registerMesh("player_mesh", modelLoader.load("Model/Maxwell_mesh.fbx"));
	assetManager.registerMesh("cat_mesh", modelLoader.load("Model/cat-box.fbx"));
	assetManager.registerMesh("floor_mesh", modelLoader.load("Model/Floor/floor.obj"));
	assetManager.registerMesh("bullet_mesh", modelLoader.load("Model/Bullet/Bullet.obj"));
	assetManager.registerMesh("Quad", modelLoader.load("Model/Quad.fbx"));


	assetManager.registerAnimation("player_idle",
		new Animation("Model/Maxwell_Idle.fbx",
			modelLoader.modelDatas[assetManager.GetModelData("player_mesh")])
	);

	assetManager.registerAnimation("player_walk",
		new Animation("Model/Maxwell.fbx",
			modelLoader.modelDatas[assetManager.GetModelData("player_mesh")])
	);

	uint16_t player = entityManager.CreateEntity().id;
	tagStore.add(player, Tag::Player);

	HealthComponent& player_health = healthStore.add(player);

	StatComponent& player_stat = statStore.add(player);
	StatModifierComponent& player_statMod = statModifierStore.add(player);

	TransformComponent& player_tf = transformStore.add(player);

	ColliderComponent& player_col = colliderStore.add(player);
	player_col.size = glm::vec3(1.f, 1.5f, 1.f);
	player_col.isStatic = false;

	uint8_t player_modelId = assetManager.GetModelData("player_mesh");

	MeshComponent& player_mesh = meshStore.add(player);
	player_mesh.mesh_id = player_modelId;

	MaterialComponent& player_mat = materialStore.add(player);

	player_mat.materialId = player_modelId;
	player_mat.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	player_mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	player_mat.shininess = 32.0f;

	AnimationComponent& player_anim = animationStore.add(player);
	player_anim.Add("player_idle", assetManager.getAnimation("player_idle"));
	player_anim.Add("player_walk", assetManager.getAnimation("player_walk"));
	player_anim.AddBool("isWalking");
	player_anim.SetTrigger("player_idle", "player_walk", "isWalking", true);
	player_anim.SetTrigger("player_walk", "player_idle", "isWalking", false);


	//animationSystem.PlayAnimation(player, assetManager.getAnimation("player_idle"));

	Entity& camera = entityManager.CreateEntity();
	CameraComponent& camera_cam = cameraStore.add(camera.id);
	tagStore.add(camera.id, Tag::Camera);
	TransformComponent& cam_tf = transformStore.add(camera.id);
	cam_tf.position = glm::vec3(0.0f, 7.0f, 10.0f);
	cam_tf.rotation = glm::vec3(-35.0f, 270.0f, 0.0f);

	Entity directLight = entityManager.CreateEntity();
	TransformComponent& directLight_tf = transformStore.add(directLight.id);
	LightComponent& directLight_light = lightStore.add(directLight.id);
	directLight_tf.position = glm::vec3(10.f, 10.f, 10.f);
	directLight_tf.rotation = glm::vec3(45.0f, 45.f, 0.f);
	directLight_light.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	directLight_light.Intensity = 1.0f;

	uint8_t block_meshId = assetManager.GetModelData("cat_mesh");

	//for (int i = 0; i < 10; i++) {
	//	uint16_t block = entityManager.CreateEntity().id;
	//	tagStore.add(block, Tag::Enemy);

	//	enemyStore.add(block);
	//	healthStore.add(block);
	//	statStore.add(block);

	//	MeshComponent& block_mesh = meshStore.add(block);
	//	block_mesh.mesh_id = block_meshId;

	//	TransformComponent& block_tf = transformStore.add(block);
	//	block_tf.position = glm::vec3(5.0f, 0.0f, 3.0f*i);

	//	ColliderComponent& block_col = colliderStore.add(block);
	//	block_col.size = glm::vec3(1, 1, 1);
	//	block_col.isStatic = true;

	//	MaterialComponent& block_mat = materialStore.add(block);
	//	float rx = (float)rand() / RAND_MAX;
	//	float rz = (float)rand() / RAND_MAX;
	//	float ra = (float)rand() / RAND_MAX;
	//	block_tf.position = glm::vec3(20.0f * rx, 0.0f, 20.0f * rz);
	//	block_tf.rotation = glm::vec3(0, 360.f * ra, 0);
	//}

	uint8_t floor_meshId = assetManager.GetModelData("floor_mesh");

	Entity& floor = entityManager.CreateEntity();
	MeshComponent& floor_mesh = meshStore.add(floor.id);
	floor_mesh.mesh_id = floor_meshId;

	TransformComponent& floor_tf = transformStore.add(floor.id);

	//// Spite Test
	//for (int i = 0; i < 10; i++) {
	//	Entity& s = entityManager.CreateEntity();
	//	SpriteComponent& s_sprite = spriteStore.add(s.id);
	//	s_sprite.textureName = "Ruri";
	//	TransformComponent& s_tf = transformStore.add(s.id);
	//	//s_tf.position = glm::vec3(0.0f, 1.0f, 0.0f);
	//	float rx = (float)rand() / RAND_MAX;
	//	float rz = (float)rand() / RAND_MAX;
	//	float ra = (float)rand() / RAND_MAX;
	//	s_tf.scale = glm::vec3(2.0f, 2.0f, 2.0f);
	//	s_tf.position = glm::vec3(20.0f * rx, 1.0f, 20.0f * rz);
	//	s_tf.rotation = glm::vec3(0, 360.f * ra, 0);
	//}

	// loop emitter — muzzle flash trail

	ParticleComponent& trail_par = particleStore.add(player);
	trail_par.type = EmitType::Loop;
	trail_par.rate = 10.0f;
	trail_par.lifeTime = 1.0f;
	trail_par.gravity = 0.0f;
	trail_par.initialSize = 0.1f;
	trail_par.sizeMultiply = 0.5f;
	trail_par.isPlaying = true;

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
	statSystem.Update(dt);
	gunSystem.Update(dt);
	enemySystem.Update(dt);
	waveSystem.Update(dt);
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
