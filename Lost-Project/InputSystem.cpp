#include "InputSystem.h"
#include "game.h"

#include "Entity.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GLMUtils.h"

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

    uint8_t cam = game.tagStore.getEntity(Tag::Camera);

	float velocity = 10.0f * dt;
	if (this->Keys[GLFW_KEY_W]) {
		player_tf.position.z -= velocity;
	}
	if (this->Keys[GLFW_KEY_S]) {
		player_tf.position.z += velocity;
	}
	if (this->Keys[GLFW_KEY_A]) {
		player_tf.position.x -= velocity;
	}
	if (this->Keys[GLFW_KEY_D]) {
		player_tf.position.x += velocity;
	}

    //for (Entity e : game.entityManager.entities) {
    //    uint8_t id = e.id;
    //    if (game.cameraStore.has(id) && game.transformStore.has(id)) {
    //        //std::cout << (unsigned)(id) << " has camera and transform" << std::endl;
    //        CameraComponent& cam = game.cameraStore.get(id);
    //        TransformComponent& tf = game.transformStore.get(id);

    //        // camera movement
    //        //std::cout << dt << std::endl;
    //        float velocity = cam.movementSpeed * dt;
    //        if (this->Keys[GLFW_KEY_W]) {
    //            tf.position += cam.front * velocity;
    //        }
    //        if (this->Keys[GLFW_KEY_S]) {
    //            tf.position -= cam.front * velocity;
    //        }
    //        if (this->Keys[GLFW_KEY_A]) {
    //            tf.position -= cam.right * velocity;
    //        }
    //        if (this->Keys[GLFW_KEY_D]) {
    //            tf.position += cam.right * velocity;
    //        }

    //        //std::cout << game.transformStore.get(id).position.x << std::endl;

    //        // camera Rotation

    //        //if (firstMouse)
    //        //{
    //        //    lastXpos = xpos;
    //        //    lastYpos = ypos;
    //        //    firstMouse = false;
    //        //}

    //        //float xoffset = xpos - lastXpos;
    //        //float yoffset = lastYpos - ypos;
    //        //std::cout << "offset : " << xoffset << " " << yoffset << endl;
    //        //lastXpos = xpos;
    //        //lastYpos = ypos;

    //        //xoffset *= 0.01f;
    //        //yoffset *= 0.01f;

    //        //cam.yaw += xoffset;
    //        //cam.pitch += yoffset;

    //        //if (cam.pitch > 89.0f)
    //        //    cam.pitch = 89.0f;
    //        //if (cam.pitch< -89.0f)
    //        //    cam.pitch = -89.0f;

    //        //xpos = lastXpos;  
    //        //ypos = lastYpos; 
    //        ////std::cout << cam.pitch << std::endl;
    //        //cam.UpdateVector();

    //        ////GLMUtils::printVec3(cam.front, "cam front : ");
    //        ////glm::vec3 direction;
    //        ////direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    //        ////direction.y = sin(glm::radians(cam.pitch));
    //        ////direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    //        ////cam.cameraFront = glm::normalize(direction);
    //    }
    //}
}