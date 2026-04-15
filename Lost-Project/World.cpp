#include "World.h"
#include "Game.h"


void World::Init() {

	Game& game = Game::getInstance();

	worldBound.origin = glm::vec3(0, 0, 0);
	worldBound.width = 100.0f;
	worldBound.height = 20.0f;
	worldBound.depth = 100.0f;


	uint16_t player = game.entityManager.CreateEntity().id;
	game.tagStore.add(player, Tag::Player);

	HealthComponent& player_health = game.healthStore.add(player);

	StatComponent& player_stat = game.statStore.add(player);
	StatModifierComponent& player_statMod = game.statModifierStore.add(player);

	TransformComponent& player_tf = game.transformStore.add(player);
	player_tf.scale = glm::vec3(0.7f, 0.7f, 0.7f);

	ColliderComponent& player_col = game.colliderStore.add(player);
	player_col.size = glm::vec3(1.5f, 1.5f, 1.5f);
	player_col.isStatic = false;

	uint8_t player_modelId = game.assetManager.GetModelData("player_mesh");

	MeshComponent& player_mesh = game.meshStore.add(player);
	player_mesh.mesh_id = player_modelId;

	MaterialComponent& player_mat = game.materialStore.add(player);

	player_mat.materialId = player_modelId;
	player_mat.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	player_mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	player_mat.shininess = 32.0f;

	AnimationComponent& player_anim = game.animationStore.add(player);
	player_anim.Add("player_idle", game.assetManager.getAnimation("player_idle"));
	player_anim.Add("player_walk", game.assetManager.getAnimation("player_walk"));
	player_anim.AddBool("isWalking");
	player_anim.SetTrigger("player_idle", "player_walk", "isWalking", true);
	player_anim.SetTrigger("player_walk", "player_idle", "isWalking", false);

	Entity& camera = game.entityManager.CreateEntity();
	CameraComponent& camera_cam = game.cameraStore.add(camera.id);
	game.tagStore.add(camera.id, Tag::Camera);
	TransformComponent& cam_tf = game.transformStore.add(camera.id);
	cam_tf.position = glm::vec3(0.0f, 7.0f, 10.0f);
	cam_tf.rotation = glm::vec3(-35.0f, 270.0f, 0.0f);

	Entity directLight = game.entityManager.CreateEntity();
	TransformComponent& directLight_tf = game.transformStore.add(directLight.id);
	LightComponent& directLight_light = game.lightStore.add(directLight.id);
	directLight_tf.position = glm::vec3(10.f, 10.f, 10.f);
	directLight_tf.rotation = glm::vec3(45.0f, 45.f, 0.f);
	directLight_light.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	directLight_light.Intensity = 1.0f;

	uint8_t block_meshId = game.assetManager.GetModelData("Tree");

	for (int i = 0; i < 50; i++) {
		uint16_t block = game.entityManager.CreateEntity().id;
		game.tagStore.add(block, Tag::Blockage);

		//enemyStore.add(block);
		//healthStore.add(block);
		//statStore.add(block);

		MeshComponent& block_mesh = game.meshStore.add(block);
		block_mesh.mesh_id = block_meshId;

		TransformComponent& block_tf = game.transformStore.add(block);
		float rx = (float)rand() / RAND_MAX;
		float rz = (float)rand() / RAND_MAX;
		float ra = (float)rand() / RAND_MAX;
		block_tf.position = glm::vec3(100.0f * rx, 1.0f, 100.0f * rz);
		block_tf.rotation = glm::vec3(0, 360.f * ra, 0);

		ColliderComponent& block_col = game.colliderStore.add(block);
		block_col.size = glm::vec3(2, 2, 2);
		block_col.isStatic = true;

		MaterialComponent& block_mat = game.materialStore.add(block);
	}

	uint8_t floor_meshId = game.assetManager.GetModelData("floor_mesh");

	Entity& floor = game.entityManager.CreateEntity();
	MeshComponent& floor_mesh = game.meshStore.add(floor.id);
	floor_mesh.mesh_id = floor_meshId;

	TransformComponent& floor_tf = game.transformStore.add(floor.id);

	ParticleComponent& trail_par = game.particleStore.add(player);
	trail_par.type = EmitType::Loop;
	trail_par.rate = 10.0f;
	trail_par.lifeTime = 1.0f;
	trail_par.gravity = 0.0f;
	trail_par.initialSize = 0.1f;
	trail_par.sizeMultiply = 0.5f;
	trail_par.isPlaying = true;
}

