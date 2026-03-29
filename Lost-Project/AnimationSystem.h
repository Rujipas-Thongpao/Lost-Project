#pragma once

#include "AnimationComponent.h"
#include "Animation.h"

class AnimationSystem {
public:
    void Update(float dt);
    void PlayAnimation(uint16_t entityId, const std::string animationName);

private:
    void CalculateBoneTransform(
        AnimationComponent& anim,
        const AssimpNodeData* node,
        glm::mat4 parentTransform);
};
