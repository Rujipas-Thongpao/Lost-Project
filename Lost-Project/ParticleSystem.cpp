#include "ParticleSystem.h"
#include "Game.h"
#include "Random.h"
#include "GLMUtils.h"
using Engine::Random;

void ParticleSystem::Update(float dt) {
    Game& game = Game::getInstance();

    // loop each particle component
    for (Entity& e : game.entityManager.entities) {
        if (e.isDestroy) continue;
        if (!game.particleStore.has(e.id)) continue;

        ParticleComponent& pc = game.particleStore.get(e.id);
        TransformComponent& tf = game.transformStore.get(e.id);

		//GLMUtils::printVec3(tf.position, "Particle emitter position : ");

        if (!pc.isPlaying) continue;

        // emit
        PlayLoop(pc, tf.position, dt);

        // loop each particle
        for (auto& p : pc.particles) {
            if (!p.active) continue;
			//std::cout << p.active << std::endl;

            p.currentLifeTime += dt;
            if (p.currentLifeTime >= pc.lifeTime) {
                p.active = false;
                continue;
            }

            // update
            p.velocity += glm::vec3(0, pc.gravity, 0) * dt;
            p.velocity *= pc.damping;
            p.position += p.velocity * dt;

            // size over lifetime
            float t = p.currentLifeTime / pc.lifeTime;
            p.size = pc.initialSize * glm::mix(1.0f, pc.sizeMultiply, t);

            // color fade
            p.color.a = 1.0f - t;
        }
    }
}


void ParticleSystem::PlayBurst(ParticleComponent& pc, glm::vec3 pos) {
    //if (pc.burstFired) return;  // only once
    // spawn all at once
    int count = (int)pc.rate;
    for (int i = 0; i < count; i++)
        Spawn(pc, pos);
    //pc.burstFired = true;
}

void ParticleSystem::PlayLoop(ParticleComponent& pc, glm::vec3 pos, float dt) {
    //if (pc.type == EmitType::Burst) {
    //    if (pc.burstFired) return;  // only once
    //    // spawn all at once
    //    int count = (int)pc.rate;
    //    for (int i = 0; i < count; i++)
    //        Spawn(pc, pos);
    //    pc.burstFired = true;
    //    return;
    //}
    // loop — spawn over time
	if (pc.type == EmitType::Burst) return;  // burst should not loop

    pc.emitTimer += dt;
    float interval = 1.0f / pc.rate;
    while (pc.emitTimer >= interval) {
        Spawn(pc, pos);
        pc.emitTimer -= interval;
    }
}

void ParticleSystem::Spawn(ParticleComponent& pc, glm::vec3 pos) {
	if (!pc.allowSpawn) return;
    // find free slot in pool
    for (auto& p : pc.particles) {
        if (p.active) continue;

        p.active = true;
        p.currentLifeTime = 0.0f;
        p.position = pos;
        p.color = pc.color;
        p.size = pc.initialSize;
        p.gravity = pc.gravity;

        // randomize velocity spread
        p.velocity = pc.velocity + glm::vec3(
            Random::range(-0.5f, 0.5f),
            Random::range(-0.5f, 0.5f),
            Random::range(-0.5f, 0.5f)
        );
        return;
    }
    // pool full — silently skip
}