#ifndef TIME_H
#define TIME_H

#include <GLFW/glfw3.h>

class Time {
public:
    Time() {}
    double lastFrame = 0.0f;
    float deltaTime = 0.0f;

    float DeltaTime() {
		double GetTime = glfwGetTime();
        deltaTime = GetTime - lastFrame;
        lastFrame = GetTime;
        return deltaTime;
    }
};
#endif


