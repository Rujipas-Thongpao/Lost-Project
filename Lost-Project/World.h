#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "GrassSystem.h"


struct Bound {
	glm::vec3 origin;
	float width, height, depth;

	bool isInBound(const glm::vec3& point) const {
		return (point.x >= origin.x - width/2 && point.x <= origin.x + width/2) &&
			   (point.y >= origin.y && point.y <= origin.y + height) &&
			   (point.z >= origin.z - depth/2 && point.z <= origin.z + depth/2);
	}
};

class World {
public:
	Bound worldBound;
	GrassSystem grassSystem ;
	void Init();
	void Update(float dt);
};
