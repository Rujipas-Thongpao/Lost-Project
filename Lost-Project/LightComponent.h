#pragma once
#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class LightComponent {
public:
	glm::vec3 Color;
	float Intensity;
};

#endif // !LIGHT_COMPONENT_H

