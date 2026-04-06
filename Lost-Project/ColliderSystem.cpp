#include "ColliderSystem.h"
#include "game.h"

#include "ColliderComponent.h"
#include "TransformComponent.h"

#include <stdio.h>
#include <iostream>
#include "GLMUtils.h"

void ColliderSystem::Init() {

}

bool aabbCheck(AABB a, AABB b) {
	return a.minX < b.maxX && a.maxX > b.minX &&
		a.minY < b.maxY && a.maxY > b.minY &&
		a.minZ < b.maxZ && a.maxZ > b.minZ;
}

void ColliderSystem::Update() {
	Game& game = Game::getInstance();

	for (uint16_t bullet : game.tagStore.getEntities(Tag::Bullet)) {
		Entity& bullet_e = game.entityManager.entities[bullet];
		if(bullet_e.isDestroy) continue;
		ColliderComponent& bullet_col = game.colliderStore.get(bullet);
		TransformComponent& bullet_tf= game.transformStore.get(bullet);
		BulletComponent& bullet_bullet = game.bulletStore.get(bullet);

		AABB bullet_aabb = bullet_col.GetAABB(bullet_tf);

		for (uint16_t block : game.tagStore.getEntities(Tag::Enemy)) {
			Entity& block_e = game.entityManager.entities[block];
			if(block_e.isDestroy) continue;
			ColliderComponent& block_col = game.colliderStore.get(block);
			TransformComponent& block_tf= game.transformStore.get(block);

			AABB block_aabb = block_col.GetAABB(block_tf);
			bool isCol = aabbCheck(bullet_aabb, block_aabb);
			if (isCol) {
				bullet_e.isDestroy = true;
				if (game.healthStore.has(block)) {
					//HealthComponent& health = game.healthStore.get(block);
					// have to get owner of the bullet
					StatComponent& owener_stat = game.statStore.get(bullet_bullet.ownerId);
					float dmg = owener_stat.finalDamage;
					game.healthSystem.Hit(block, dmg);
				}
				//std::cout << (unsigned)b_id<< " " <<b.isDestroy << endl;
			}
		}
	}

	//for (Entity a : game.entityManager.entities) {
	//	uint16_t a_id = a.id;
	//	if(a.isDestroy) continue;
	//	if (game.colliderStore.has(a_id) && game.transformStore.has(a_id)) {

	//		ColliderComponent& a_col = game.colliderStore.get(a_id);
	//		TransformComponent& a_tf= game.transformStore.get(a_id);

	//		AABB a_aabb = a_col.GetAABB(a_tf);

	//		for (Entity& b : game.entityManager.entities) {
	//			uint16_t b_id = b.id;
	//			if(b.isDestroy) continue;

	//			if (a_id == b_id) continue;

	//			if (game.colliderStore.has(b_id) && game.transformStore.has(b_id)) {

	//				ColliderComponent& b_col = game.colliderStore.get(b_id);
	//				TransformComponent& b_tf= game.transformStore.get(b_id);

	//				AABB b_aabb = b_col.GetAABB(b_tf);
	//				bool isCol = aabbCheck(a_aabb, b_aabb);
	//				if (isCol) {
	//					b.isDestroy = true;
	//					//std::cout << (unsigned)b_id<< " " <<b.isDestroy << endl;
	//				}
	//			}
	//		}
	//	}
	//}
}

