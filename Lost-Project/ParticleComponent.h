#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <String>

enum class EmitType { Loop, Burst };

struct Particle {
    bool active = false;
    float currentLifeTime = 0.0f;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float size;
    float gravity;
};

class ParticleComponent {
public:
    // emission
    EmitType  type = EmitType::Loop;
    float     rate = 10.0f;   // particles per second
    // burst = rate fired all in first second then stops
    // loop  = rate fired continuously

    float     emitTimer = 0.0f;
    bool      isPlaying = false;
    bool 	  allowSpawn = true;
    bool      burstFired = false;  // prevent burst repeating

    // per particle config
	uint16_t  origin;
    float     lifeTime = 1.0f;
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec3 velocity = glm::vec3(0.0f, 1.0f, 0.0f);
    float     damping = 0.98f;   // velocity multiplier per frame
    float     gravity = -9.8f;
    float initialSize = 0.1;
    float     sizeMultiply = 0.5f;    // size * sizeMultiply over lifetime
    std::string sprite = "Circle";

    // pool
    static const int MAX_PARTICLES = 200;
    Particle particles[MAX_PARTICLES];
};
