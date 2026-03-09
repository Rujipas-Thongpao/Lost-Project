#include "CameraComponent.h"
#include <stdio.h>
#include <iostream>
#include "GLMUtils.h"

glm::mat4 CameraComponent::GetViewMatrix(const TransformComponent& tf) {
    glm::vec3 f;
    f.x = cos(glm::radians(tf.rotation.y)) * cos(glm::radians(tf.rotation.x));
    f.y = sin(glm::radians(tf.rotation.x));
    f.z = sin(glm::radians(tf.rotation.y)) * cos(glm::radians(tf.rotation.x));

    front = glm::normalize(f);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    return glm::lookAt(tf.position, tf.position + front, up);
}

glm::mat4 CameraComponent::GetProjectionMatrix(float aspectRatio) {
    return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
}

void CameraComponent::UpdateVector() {
    //glm::vec3 f;
    //f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //f.y = sin(glm::radians(pitch));
    //f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    ////std::cout << pitch << std::endl;
    ////GLMUtils::printVec3(f, "f");

    //front = glm::normalize(f);
    //right = glm::normalize(glm::cross(front, worldUp));
    //up = glm::normalize(glm::cross(right, front));
}

