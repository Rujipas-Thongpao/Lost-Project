#include "InputSystem.h"
#include "game.h"

#include "Entity.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GLMUtils.h"
#include "StatComponent.h"
#include "StatHelper.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

void InputSystem::Init() {
    firstMouse = true;
}

void InputSystem::Update(float dt) {
    Game& game = Game::getInstance();

    uint8_t player = game.tagStore.getEntity(Tag::Player);
    TransformComponent& player_tf = game.transformStore.get(player);
    StatComponent& player_stat = game.statStore.get(player);

    uint8_t cam = game.tagStore.getEntity(Tag::Camera);

	float velocity = player_stat.finalSpeed * dt;
    float angular = 500.0f * dt;

	bool isWalking = false;
	bool isRunning = false;

	if (game.entityManager.entities[player].isDestroy) return;

    if (this->Keys[GLFW_KEY_LEFT_SHIFT] && player_stat.currentStamina > 0.1f) {
		velocity = player_stat.finalRunSpeed  * dt;
		isRunning = true;
    }

	if (this->Keys[GLFW_KEY_W]) {
		player_tf.position += velocity * player_tf.getFront();
        isWalking = true;
	}
	if (this->Keys[GLFW_KEY_S]) {
		player_tf.position -= velocity * player_tf.getFront();
        isWalking = true;
	}
	if (this->Keys[GLFW_KEY_A]) {
        player_tf.rotation.y -= angular;
        //isWalking = true;
	}
	if (this->Keys[GLFW_KEY_D]) {
        player_tf.rotation.y += angular;
        //isWalking = true;
	}

    if (this->Keys[GLFW_KEY_SPACE]) {
        game.gunSystem.Shoot();
    }

    if (isWalking && isRunning) {
		player_stat.currentStamina -= 2.0f * dt;
		player_stat.currentStamina = std::max(player_stat.currentStamina, 0.0f);
    }
    else {
		player_stat.currentStamina += dt;
		player_stat.currentStamina = std::min(player_stat.currentStamina, player_stat.stamina);
    }

    AnimationComponent& player_anim = game.animationStore.get(player);
    player_anim.SetBool("isWalking", isWalking);
	ParticleComponent& trail = game.particleStore.get(player);
	trail.allowSpawn = isWalking;


    if(this->Keys[GLFW_KEY_UP]) {
        // equip sword — adds flat +2 damage
		StatModifierComponent& mods = game.statModifierStore.get(player);
        StatHelper::AddModifier(mods, {
            StatType::Damage, ModifyType::Add, 2.0f, "sword"
            });
	}

    if(this->Keys[GLFW_KEY_DOWN]) {
        // equip sword — adds flat +2 damage
		StatModifierComponent& mods = game.statModifierStore.get(player);
        StatHelper::AddModifier(mods, {
            StatType::Health, ModifyType::Add, 2.0f, "sword"
            });
	}
}