#pragma once
#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TransformComponent.h"

class CameraComponent {
public:
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw = -90.0f;
    float pitch = 0.0f;
    float movementSpeed = 100000.0f;
    float mouseSensitivity = 0.1f;
    float zoom = 45.0f;
    bool  isActive = true;

    CameraComponent() {
        updateVectors();
    }

    glm::mat4 GetViewMatrix(const TransformComponent& tf) const {
        return glm::lookAt(tf.position, tf.position + front, up);
    }

    glm::mat4 GetProjectionMatrix(float aspectRatio = 1) const {
        return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
    }

    void updateVectors() {
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};
#endif // !CAMERA_COMPONENT_H
