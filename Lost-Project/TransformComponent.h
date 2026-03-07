#pragma once
#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H


#include <stdio.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

class TransformComponent {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	TransformComponent() : position(0.0f), rotation(0.0f), scale(1.0f) {}
};

#endif // !TRANSFORM_COMPONENT_H


