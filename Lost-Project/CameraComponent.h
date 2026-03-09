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

    glm::mat4 GetViewMatrix(const TransformComponent& tf);
    glm::mat4 GetProjectionMatrix(float aspectRatio = 1);
    void UpdateVector();
};

#endif // !CAMERA_COMPONENT_H
