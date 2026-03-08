#include "InputSystem.h"
#include "game.h"

#include "Entity.h"
#include "CameraComponent.h"

#include <stdio.h>
#include <iostream>

void InputSystem::Init() {

}

void InputSystem::Update(float dt) {
    Game& game = Game::getInstance();

    for (Entity e : game.entityManager.entities) {
        uint8_t id = e.id;
        if (game.cameraStore.has(id) && game.transformStore.has(id)) {
            //std::cout << (unsigned)(id) << " has camera and transform" << std::endl;
            CameraComponent& cam = game.cameraStore.get(id);
            TransformComponent& tf = game.transformStore.get(id);

            // camera movement
            float velocity = cam.movementSpeed * dt;
            if (this->Keys[GLFW_KEY_W]) {
                tf.position += cam.front * velocity;
            }
            if (this->Keys[GLFW_KEY_S]) {
                tf.position -= cam.front * velocity;
            }
            if (this->Keys[GLFW_KEY_A]) {
                tf.position -= cam.right * velocity;
            }
            if (this->Keys[GLFW_KEY_D]) {
                tf.position += cam.right * velocity;
            }

            //std::cout << game.transformStore.get(id).position.x << std::endl;

            // camera Rotation

            if (firstMouse)
            {
                lastXpos = xpos;
                lastYpos = ypos;
                firstMouse = false;
            }

            float xoffset = xpos - lastXpos;
            float yoffset = lastYpos- ypos;
            lastXpos = xpos;
            lastYpos = ypos;

            xoffset *= sensitivity;
            yoffset *= sensitivity;

            cam.yaw += xoffset;
            cam.pitch += yoffset;

            if (cam.pitch > 89.0f)
                cam.pitch = 89.0f;
            if (cam.pitch< -89.0f)
                cam.pitch = -89.0f;

            cam.updateVectors();
            //glm::vec3 direction;
            //direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
            //direction.y = sin(glm::radians(cam.pitch));
            //direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
            //cam.cameraFront = glm::normalize(direction);
        }
    }
}