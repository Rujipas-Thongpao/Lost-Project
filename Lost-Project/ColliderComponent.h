#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "TransformComponent.h"

struct AABB {
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
};

class ColliderComponent {
public:
	glm::vec3 size;
	glm::vec3 offset;
	AABB GetAABB(TransformComponent& tf);
};