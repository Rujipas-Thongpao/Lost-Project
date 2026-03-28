#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Animation.h"

struct AnimationComponent {
    Animation* currentAnimation = nullptr;
    float      currentTime = 0.0f;
    std::vector<glm::mat4> finalBoneMatrices;

    AnimationComponent() {
        finalBoneMatrices.resize(100, glm::mat4(1.0f));
    }
};
