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

    auto& allEntities = game.entityManager.entities;

    for (int i = 0; i < allEntities.size(); i++) {
        if (allEntities[i].isDestroy) continue;
        if (!game.colliderStore.has(i)) continue;

        for (int j = i + 1; j < allEntities.size(); j++) {
            if (allEntities[j].isDestroy) continue;
            if (!game.colliderStore.has(j)) continue;

            ColliderComponent& colA = game.colliderStore.get(i);
            ColliderComponent& colB = game.colliderStore.get(j);
            TransformComponent& tfA = game.transformStore.get(i);
            TransformComponent& tfB = game.transformStore.get(j);

            AABB aabbA = colA.GetAABB(tfA);
            AABB aabbB = colB.GetAABB(tfB);

            if (!aabbCheck(aabbA, aabbB)) continue;

            if (colA.isStatic != colB.isStatic) {
                uint16_t dynamicId = colA.isStatic ? j : i;
                uint16_t staticId = colA.isStatic ? i : j;

                TransformComponent& dynTf = game.transformStore.get(dynamicId);
                TransformComponent& staticTf = game.transformStore.get(staticId);
                AABB dynAABB = game.colliderStore.get(dynamicId).GetAABB(dynTf);
                AABB staticAABB = game.colliderStore.get(staticId).GetAABB(staticTf);

                float overlapX = std::min(dynAABB.maxX, staticAABB.maxX) - std::max(dynAABB.minX, staticAABB.minX);
                float overlapY = std::min(dynAABB.maxY, staticAABB.maxY) - std::max(dynAABB.minY, staticAABB.minY);
                float overlapZ = std::min(dynAABB.maxZ, staticAABB.maxZ) - std::max(dynAABB.minZ, staticAABB.minZ);

                if (overlapX < overlapY && overlapX < overlapZ) {
                    float dir = (dynTf.position.x < staticTf.position.x) ? -1.f : 1.f;
                    dynTf.position.x += dir * overlapX;
                }
                else if (overlapY < overlapX && overlapY < overlapZ) {
                    float dir = (dynTf.position.y < staticTf.position.y) ? -1.f : 1.f;
                    dynTf.position.y += dir * overlapY;
                }
                else {
                    float dir = (dynTf.position.z < staticTf.position.z) ? -1.f : 1.f;
                    dynTf.position.z += dir * overlapZ;
                }
            }

            bool aIsEnemy = game.tagStore.has(i, Tag::Enemy);
            bool bIsEnemy = game.tagStore.has(j, Tag::Enemy);
            bool aIsBullet = game.tagStore.has(i, Tag::Bullet);
            bool bIsBullet = game.tagStore.has(j, Tag::Bullet);
            bool aIsPlayer = game.tagStore.has(i, Tag::Player);
            bool bIsPlayer = game.tagStore.has(j, Tag::Player);

            uint16_t bulletId = 0, enemyId = 0, playerId = 0;
            bool isBulletEnemy = (aIsBullet && bIsEnemy) || (bIsBullet && aIsEnemy);
            bool isEnemyPlayer = (aIsEnemy && bIsPlayer) || (bIsEnemy && aIsPlayer);

            if (isBulletEnemy) {
                bulletId = aIsBullet ? i : j;
                enemyId = aIsEnemy ? i : j;

                game.entityManager.entities[bulletId].isDestroy = true;
                if (game.healthStore.has(enemyId)) {
                    BulletComponent& bullet = game.bulletStore.get(bulletId);
                    StatComponent& ownerStat = game.statStore.get(bullet.ownerId);
                    game.healthSystem.Hit(enemyId, ownerStat.finalDamage);
                }
            }

            if (isEnemyPlayer) {
                enemyId = aIsEnemy ? i : j;
                playerId = aIsPlayer ? i : j;
                // do damage to player here
                game.healthSystem.Hit(playerId, 1);
            }
        }
    }
}

//void ColliderSystem::Update() {
//	Game& game = Game::getInstance();
//
//	for (uint16_t bullet : game.tagStore.getEntities(Tag::Bullet)) {
//		Entity& bullet_e = game.entityManager.entities[bullet];
//		if(bullet_e.isDestroy) continue;
//		ColliderComponent& bullet_col = game.colliderStore.get(bullet);
//		TransformComponent& bullet_tf= game.transformStore.get(bullet);
//		BulletComponent& bullet_bullet = game.bulletStore.get(bullet);
//
//		AABB bullet_aabb = bullet_col.GetAABB(bullet_tf);
//
//		for (uint16_t block : game.tagStore.getEntities(Tag::Enemy)) {
//			Entity& block_e = game.entityManager.entities[block];
//			if(block_e.isDestroy) continue;
//			ColliderComponent& block_col = game.colliderStore.get(block);
//			TransformComponent& block_tf= game.transformStore.get(block);
//
//			AABB block_aabb = block_col.GetAABB(block_tf);
//			bool isCol = aabbCheck(bullet_aabb, block_aabb);
//			if (isCol) {
//				bullet_e.isDestroy = true;
//				if (game.healthStore.has(block)) {
//					//HealthComponent& health = game.healthStore.get(block);
//					// have to get owner of the bullet
//					StatComponent& owener_stat = game.statStore.get(bullet_bullet.ownerId);
//					float dmg = owener_stat.finalDamage;
//					game.healthSystem.Hit(block, dmg);
//				}
//				//std::cout << (unsigned)b_id<< " " <<b.isDestroy << endl;
//			}
//		}
//	}
//
//	//for (Entity a : game.entityManager.entities) {
//	//	uint16_t a_id = a.id;
//	//	if(a.isDestroy) continue;
//	//	if (game.colliderStore.has(a_id) && game.transformStore.has(a_id)) {
//
//	//		ColliderComponent& a_col = game.colliderStore.get(a_id);
//	//		TransformComponent& a_tf= game.transformStore.get(a_id);
//
//	//		AABB a_aabb = a_col.GetAABB(a_tf);
//
//	//		for (Entity& b : game.entityManager.entities) {
//	//			uint16_t b_id = b.id;
//	//			if(b.isDestroy) continue;
//
//	//			if (a_id == b_id) continue;
//
//	//			if (game.colliderStore.has(b_id) && game.transformStore.has(b_id)) {
//
//	//				ColliderComponent& b_col = game.colliderStore.get(b_id);
//	//				TransformComponent& b_tf= game.transformStore.get(b_id);
//
//	//				AABB b_aabb = b_col.GetAABB(b_tf);
//	//				bool isCol = aabbCheck(a_aabb, b_aabb);
//	//				if (isCol) {
//	//					b.isDestroy = true;
//	//					//std::cout << (unsigned)b_id<< " " <<b.isDestroy << endl;
//	//				}
//	//			}
//	//		}
//	//	}
//	//}
//}

