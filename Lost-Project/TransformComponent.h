#pragma once
#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H


#include <stdio.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

class TransformComponent {
public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	TransformComponent() : position(0.0f), rotation(0.0f), scale(1.0f) {}
	glm::vec3 getFront() {
		float yaw = glm::radians(rotation.y);
		float pitch = glm::radians(rotation.x);

		glm::vec3 front;
		front.x = cos(yaw) * cos(pitch);
		front.y = sin(pitch);
		front.z = sin(yaw) * cos(pitch);
		front = glm::normalize(front);
		return front;
	}
};

#endif // !TRANSFORM_COMPONENT_H


