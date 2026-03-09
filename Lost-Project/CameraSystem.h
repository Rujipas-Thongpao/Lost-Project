#pragma once
#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class CameraSystem {
public:
	void Init();
	void Update(float dt);
private:
	glm::vec3 initialOffset;
};

#endif
