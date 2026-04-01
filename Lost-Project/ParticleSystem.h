#pragma once
#include "ParticleComponent.h"

class ParticleSystem {
public:

    void Update(float dt);
	void PlayBurst(ParticleComponent& pc, glm::vec3 pos);
    void PlayLoop(ParticleComponent& pc, glm::vec3 pos, float dt);
    void Spawn(ParticleComponent& pc, glm::vec3 pos);
};
