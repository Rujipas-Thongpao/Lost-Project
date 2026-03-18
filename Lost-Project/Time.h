// Time.h
#pragma once
#include <GLFW/glfw3.h>

class Time {
public:
    static Time& getInstance() {
        static Time instance;
        return instance;
    }
    Time(const Time&) = delete;
    Time& operator=(const Time&) = delete;

    void tick() {
        float current = (float)glfwGetTime();
        deltaTime = current - lastFrame;
        lastFrame = current;
        elapsed = current;
        frameCount++;
    }

    float getDeltaTime()  const { return deltaTime; }
    float getElapsed()    const { return elapsed; }
    float getFPS()        const { return deltaTime > 0 ? 1.0f / deltaTime : 0; }
    int   getFrameCount() const { return frameCount; }

private:
    Time() {}
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float elapsed = 0.0f;
    int   frameCount = 0;
};