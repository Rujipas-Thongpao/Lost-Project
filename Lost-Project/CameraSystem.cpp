#include "CameraSystem.h"
#include "Game.h"
#include "TransformComponent.h"

void CameraSystem::Init() {
	Game& game = Game::getInstance();
	uint16_t player = game.tagStore.getEntity(Tag::Player);
	uint16_t camera = game.tagStore.getEntity(Tag::Camera);

	TransformComponent& player_tf = game.transformStore.get(player);
	TransformComponent& camera_tf = game.transformStore.get(camera);

	initialOffset = camera_tf.position - player_tf.position;
}

void CameraSystem::Update(float dt) {
	Game& game = Game::getInstance();
	uint16_t player = game.tagStore.getEntity(Tag::Player);
	uint16_t camera = game.tagStore.getEntity(Tag::Camera);

	TransformComponent& player_tf = game.transformStore.get(player);
	TransformComponent& camera_tf = game.transformStore.get(camera);

	camera_tf.position = player_tf.position + initialOffset;
}
