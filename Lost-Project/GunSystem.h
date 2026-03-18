#pragma once

class GunSystem {
public:
	float maxCooldown;
	float currentCooldown;
	void Init();
	void Shoot();
	void Update(float dt);
};
