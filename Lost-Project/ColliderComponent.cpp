#include "ColliderComponent.h"

AABB ColliderComponent::GetAABB(TransformComponent& tf) {
	AABB aabb;
	aabb.minX = tf.position.x - size.x/2;
	aabb.maxX = tf.position.x + size.x/2;

	aabb.minY = tf.position.y - size.y/2;
	aabb.maxY = tf.position.y + size.y/2;

	aabb.minZ = tf.position.z - size.z/2;
	aabb.maxZ = tf.position.z + size.z/2;
	return aabb;
}

