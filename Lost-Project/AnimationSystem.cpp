#include "AnimationSystem.h"
#include "game.h"

void AnimationSystem::Update(float dt) {
	Game& game = Game::getInstance();

	for (Entity& e : game.entityManager.entities) {
		if (e.isDestroy) continue;
		if (!game.animationStore.has(e.id)) continue;
		if (!game.meshStore.has(e.id)) continue;

		AnimationComponent& anim = game.animationStore.get(e.id);
		if (!anim.currentAnimation) continue;

		// advance time
		anim.currentTime += anim.currentAnimation->GetTicksPerSecond() * dt;
		anim.currentTime = fmod(anim.currentTime, anim.currentAnimation->GetDuration());

		// recalculate bone matrices
		CalculateBoneTransform(
			anim,
			&anim.currentAnimation->GetRootNode(),
			glm::mat4(1.0f)
		);
	}
}

void AnimationSystem::PlayAnimation(uint16_t entityId, Animation* animation) {
	Game& game = Game::getInstance();
	AnimationComponent& anim = game.animationStore.get(entityId);
	anim.currentAnimation = animation;
	anim.currentTime = 0.0f;
}

void AnimationSystem::CalculateBoneTransform(
	AnimationComponent& anim,
	const AssimpNodeData* node,
	glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* bone = anim.currentAnimation->FindBone(nodeName);
	if (bone) {
		bone->Update(anim.currentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	auto& boneInfoMap = anim.currentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
		int       index = boneInfoMap.at(nodeName).id;
		glm::mat4 offset = boneInfoMap.at(nodeName).offset;
		anim.finalBoneMatrices[index] = globalTransform * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(anim, &node->children[i], globalTransform);
}
