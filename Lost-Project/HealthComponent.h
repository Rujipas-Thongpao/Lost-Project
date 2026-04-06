#pragma once

struct HealthComponent{
	int maxHealth = 3;
	int currentHealth = 3;
	float coolDownHit =0.3f;
	float currentCooldownHit = 0.0f;
	bool hasInit = false;
};
