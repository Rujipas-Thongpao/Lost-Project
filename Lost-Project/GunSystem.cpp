#include "GunSystem.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "BulletComponent.h"
#include "Entity.h";
#include "GLMUtils.h"
#include "ModelLoader.h"

#include "game.h"
#include "Asset.h"
#include <stdio.h>
#include <iostream>


void GunSystem::Init() {
	maxCooldown = .1f;
	currentCooldown = 0.f;
}

void GunSystem::Shoot() {
	if (currentCooldown < maxCooldown) return;
	currentCooldown = 0.0f;

	Game& game = Game::getInstance();
	Assets& assetManager = Assets::getInstance();

	uint8_t player = game.tagStore.getEntity(Tag::Player);
	TransformComponent& player_tf = game.transformStore.get(player);
	glm::vec3 player_tf_front = player_tf.getFront();

	glm::vec3 spawnPos = player_tf.position;
	glm::vec3 spawnFront = player_tf.getFront();

	Entity bullet = game.entityManager.CreateEntity();
	uint8_t bullet_id = bullet.id;
	game.tagStore.add(bullet_id, Tag::Bullet);

	TransformComponent& bullet_tf = game.transformStore.add(bullet_id);
	bullet_tf.position = spawnPos + glm::vec3(0, 0.5f, 0);
	bullet_tf.scale = glm::vec3(.4f, .4f, .4f);

	BulletComponent& b = game.bulletStore.add(bullet_id);
	b.direction = spawnFront;

	ColliderComponent& bullet_col = game.colliderStore.add(bullet_id);
	bullet_col.size = glm::vec3(2.f, 2.f, 2.f);

	MeshComponent& bullet_mesh = game.meshStore.add(bullet_id);
	bullet_mesh.mesh_id = assetManager.getMesh("bullet_mesh");
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