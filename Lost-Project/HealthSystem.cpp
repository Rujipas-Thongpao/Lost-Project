#include "HealthSystem.h"
#include "Game.h"
#include <algorithm>
#include <iostream>
#include <string_view>


void HealthSystem::Init() {
	Game& game = Game::getInstance();

	for(Entity & e : game.entityManager.entities) {
		if(e.isDestroy) continue;
		if (!game.healthStore.has(e.id)) continue;
		if (!game.statStore.has(e.id)) continue;

		HealthComponent& hc = game.healthStore.get(e.id);
		StatComponent& sc = game.statStore.get(e.id);

		// init health based on stat component
		hc.maxHealth = sc.baseHealth;
		hc.currentHealth = sc.baseHealth;
		hc.hasInit = true;
	}
}

void HealthSystem::Update(float dt) {
	Game& game = Game::getInstance();

	for(Entity & e : game.entityManager.entities) {
		if(e.isDestroy) continue;
		if (!game.healthStore.has(e.id)) continue;
		if (!game.statStore.has(e.id)) continue;

		HealthComponent& hc = game.healthStore.get(e.id);
		StatComponent& sc = game.statStore.get(e.id);
		if (!hc.hasInit) {

			hc.maxHealth = sc.baseHealth;
			hc.currentHealth = sc.baseHealth;

			hc.hasInit = true;
			continue;
		}
		if (hc.currentCooldownHit > 0) {
			hc.currentCooldownHit -= dt;
		}

		if(hc.currentHealth <= 0) {
			e.isDestroy = true;
			continue;
		}

		// regenerate health based on stat component

		hc.currentHealth += sc.regenHealthRate * dt; 
		hc.currentHealth = std::min(hc.currentHealth, hc.maxHealth);
	}
}

void HealthSystem::Hit(uint16_t receiver, int dmg) {
	Game& game = Game::getInstance();
	if (!game.healthStore.has(receiver))
		return;

	if (!game.statStore.has(receiver))
		return;

	HealthComponent& hc = game.healthStore.get(receiver);
	StatComponent& sc = game.statStore.get(receiver);

	if (hc.currentCooldownHit > 0) return; 
	hc.currentHealth -= dmg * (1.0f-sc.damageReduction);
	hc.currentCooldownHit = hc.coolDownHit;
	std::cout << "Entity " << receiver << " got hit! Current health: " << hc.currentHealth << std::endl;
}

