#include "HealthSystem.h"
#include "Game.h"


void HealthSystem::Init() {
}

void HealthSystem::Update(float dt) {
	Game& game = Game::getInstance();

	for(Entity & e : game.entityManager.entities) {
		if(e.isDestroy) continue;
		if (!game.healthStore.has(e.id)) continue;

		HealthComponent& hc = game.healthStore.get(e.id);
		if (hc.currentCooldownHit > 0) {
			hc.currentCooldownHit -= dt;
		}

		if(hc.currentHealth <= 0) {
			e.isDestroy = true;
		}
	}

}

void HealthSystem::Hit(uint16_t receiver) {
	Game& game = Game::getInstance();
	if (!game.healthStore.has(receiver))
		return;

	HealthComponent& hc = game.healthStore.get(receiver);

	if (hc.currentCooldownHit > 0) return; 
	hc.currentHealth -= 1;
	hc.currentCooldownHit = hc.coolDownHit;
	std::cout << "Entity " << (unsigned) receiver << " hit! Current health: " << hc.currentHealth << std::endl;
}

