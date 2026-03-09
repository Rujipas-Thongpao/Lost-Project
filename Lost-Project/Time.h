#ifndef TIME_H
#define TIME_H

#include <GLFW/glfw3.h>

class Time {
public:
    Time() {}
    double lastTime = 0.0f;
    float deltaTime = 0.0f;

    float DeltaTime() {
		double GetTime = glfwGetTime();
        deltaTime = GetTime - lastTime;
        lastTime = GetTime;
        return deltaTime;
    }
};
#endif


