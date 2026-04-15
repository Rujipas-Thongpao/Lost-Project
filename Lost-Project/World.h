#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>


struct Bound {
	glm::vec3 origin;
	float width, height, depth;

	bool isInBound(const glm::vec3& point) const {
		return (point.x >= origin.x && point.x <= origin.x + width) &&
			   (point.y >= origin.y && point.y <= origin.y + height) &&
			   (point.z >= origin.z && point.z <= origin.z + depth);
	}
};

class World {
public:
	Bound worldBound;
	void Init();
	void Update(float dt);
};
