#include "EnemySystem.h"
#include "EnemyComponent.h"
#include "game.h"

void EnemySystem::Init() {
}

void EnemySystem::Update(float dt) {
	Game& game = Game::getInstance();
	for (uint16_t enemy : game.tagStore.getEntities(Tag::Enemy)) {
		Entity& enemy_e = game.entityManager.entities[enemy];
		if(enemy_e.isDestroy) continue;
		EnemyComponent& enemy_c = game.enemyStore.get(enemy);
		TransformComponent& enemy_tf = game.transformStore.get(enemy);
		uint16_t player = game.tagStore.getEntity(Tag::Player);
		TransformComponent& player_tf = game.transformStore.get(player);
		glm::vec3 direction = player_tf.position - enemy_tf.position;
		float distance = glm::length(direction);
		if (distance > 0.1f) {
			direction = glm::normalize(direction);
			enemy_tf.position += direction * enemy_c.speed * dt;
			enemy_tf.rotation.y = atan2(direction.z, direction.x) * 180.0f / 3.14159f - 90.0f;
		}
	}
}