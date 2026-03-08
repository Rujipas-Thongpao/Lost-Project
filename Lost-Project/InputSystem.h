#pragma once
#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <stdio.h>

class InputSystem {
public:
    bool         Keys[1024];
	bool firstMouse = true;
	double		lastXpos = 0.0, xpos = 0.0, lastYpos = 0.0, ypos = 0.0;
	float sensitivity = 0.1f;
	void Init();
	void Update(float dt);
};

#endif
