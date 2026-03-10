#include "ColliderSystem.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "game.h"
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

	for (Entity a : game.entityManager.entities) {
		uint8_t a_id = a.id;
		if (game.colliderStore.has(a_id) && game.transformStore.has(a_id)) {

			ColliderComponent& a_col = game.colliderStore.get(a_id);
			TransformComponent& a_tf= game.transformStore.get(a_id);

			AABB a_aabb = a_col.GetAABB(a_tf);

			for (Entity b : game.entityManager.entities) {
				uint8_t b_id = b.id;

				if (a_id == b_id) continue;

				if (game.colliderStore.has(b_id) && game.transformStore.has(b_id)) {

					ColliderComponent& b_col = game.colliderStore.get(b_id);
					TransformComponent& b_tf= game.transformStore.get(b_id);

					AABB b_aabb = b_col.GetAABB(b_tf);
					bool isCol = aabbCheck(a_aabb, b_aabb);
				}
			}
		}
	}
}

