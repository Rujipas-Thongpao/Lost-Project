#include "GunSystem.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "BulletComponent.h"
#include "Entity.h";
#include "GLMUtils.h"
#include "ModelLoader.h"
#include "Random.h"
using namespace Engine;

#include "game.h"
#include "Asset.h"
#include <stdio.h>
#include <iostream>


void GunSystem::Init() {
	maxCooldown = .1f;
	currentCooldown = 0.f;
}

void GunSystem::Shoot() {


	Game& game = Game::getInstance();
	Assets& assetManager = game.assetManager;

	uint16_t player = game.tagStore.getEntity(Tag::Player);

	maxCooldown = 1.0f / game.statStore.get(player).finalAttackSpeed;

	if (currentCooldown < maxCooldown) return;
	currentCooldown = 0.0f;

	TransformComponent& player_tf = game.transformStore.get(player);
	StatComponent& player_stat = game.statStore.get(player);
	glm::vec3 player_tf_front = player_tf.getFront();

	glm::vec3 spawnPos = player_tf.position;
	glm::vec3 spawnFront = player_tf.getFront();

	for (int i = 0; i < player_stat.finalBulletCount; i++) {
		Entity bullet = game.entityManager.CreateEntity();
		uint16_t bullet_id = bullet.id;
		game.tagStore.add(bullet_id, Tag::Bullet);

		TransformComponent& bullet_tf = game.transformStore.add(bullet_id);
		bullet_tf.position = spawnPos + glm::vec3(0, 0.5f, 0);
		bullet_tf.scale = glm::vec3(.4f, .4f, .4f);

		BulletComponent& b = game.bulletStore.add(bullet_id);
		b.direction = spawnFront + glm::vec3(Random::range(-0.3f, 0.3f), 0, Random::range(-0.3f, 0.3f));
		b.ownerId = player;

		ColliderComponent& bullet_col = game.colliderStore.add(bullet_id);
		bullet_col.size = glm::vec3(2.f, 2.f, 2.f);

		MeshComponent& bullet_mesh = game.meshStore.add(bullet_id);
		bullet_mesh.mesh_id = assetManager.GetModelData("bullet_mesh");
	}

	Entity& particle = game.entityManager.CreateEntity();
	ParticleComponent& muzzle = game.particleStore.add(particle.id);
	muzzle.type = EmitType::Burst;
	muzzle.rate = 3.0f;
	muzzle.lifeTime = 1.0f;
	muzzle.gravity = 0.0f;
	muzzle.initialSize = 0.1f;
	muzzle.sizeMultiply = 0.5f;
	muzzle.isPlaying = true;
	muzzle.velocity = (player_tf_front * 5.f + glm::vec3(0, 1.f, 0)) * 1.0f;
	game.particleSystem.PlayBurst(muzzle, player_tf.position + player_tf.getFront() * 0.5f + glm::vec3(0, 0.5f, 0));
	//ParticleComponent& muzzle = game.particleStore.get(player);
	//game.particleSystem.PlayBurst(muzzle, spawnPos + spawnFront * 0.5f + glm::vec3(0, 0.5f, 0));
}

void GunSystem::Update(float dt) {
	currentCooldown += dt;
	Game& game = Game::getInstance();
	for (Entity& e : game.entityManager.entities) {
		bool hasBullet = game.bulletStore.has(e.id);
		bool hasTf = game.transformStore.has(e.id);
		//std::cout << (unsigned)e.id << " " << hasBullet <<  " " << hasTf <<   std::endl;
		if (!hasBullet) continue;
		if (!hasTf) continue;
		//if (!game.colliderStore.has(e.id)) continue;

		BulletComponent& bullet = game.bulletStore.get(e.id);
		TransformComponent& bullet_tf = game.transformStore.get(e.id);
		//ColliderComponent& bullet_col = game.colliderStore.get(e.id);

		bullet_tf.position += bullet.direction * 100.f * dt;
		//GLMUtils::printVec3(bullet_tf.position);
	}
}