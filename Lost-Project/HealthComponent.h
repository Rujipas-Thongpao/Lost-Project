#pragma once

struct HealthComponent{
	int maxHealth = 3;
	int currentHealth = 3;
	float coolDownHit = 1.0f;
	float currentCooldownHit = 0.0f;
};
