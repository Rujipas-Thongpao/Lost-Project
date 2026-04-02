#pragma once
#include "HealthComponent.h"
#include <cstdint>

class HealthSystem {
public:
	void Init();
	void Update(float dt);
	void Hit(uint16_t receiver);
};
