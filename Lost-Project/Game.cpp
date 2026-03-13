#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "game.h"
#include "MaterialComponent.h"
#include "RendererSystem.h"
#include <cstdlib>


Game& Game::getInstance() {
		static Game instance;
		return instance;
}

Game::Game()
    : State(GAME_ACTIVE)
{

}

Game::~Game()
{

}

void Game::Init()
{
	transformSystem = TransformSystem();
	entityManager.Init();
	rendererSystem.Init();
	inputSystem.Init();
	colliderSystem.Init();

	Entity& player = entityManager.CreateEntity();
	tagStore.add(player.id, Tag::Player);

	ColliderComponent& player_col = colliderStore.add(player.id);
	TransformComponent& player_tf = transformStore.add(player.id);
	modelLoader.load(player.id, "Model/Maxwell/Maxwell.obj", false);
	MaterialComponent& player_mat = materialStore.get(player.id);
	player_col.size = glm::vec3(1.f, 1.5f, 1.f);
	player_col.isStatic = false;

	player_mat.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	player_mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	player_mat.shininess = 32.0f;

	Entity camera = entityManager.CreateEntity();
	cameraStore.add(camera.id);
	tagStore.add(camera.id, Tag::Camera);
	TransformComponent& cam_tf = transformStore.add(camera.id);
	cam_tf.position = glm::vec3(0.0f, 3.0f, 10.0f);
	cam_tf.rotation = glm::vec3(-10.0f, 270.0f, 0.0f);

	Entity directLight = entityManager.CreateEntity();
	TransformComponent& directLight_tf = transformStore.add(directLight.id);
	LightComponent& directLight_light = lightStore.add(directLight.id);
	directLight_tf.position = glm::vec3(10.f, 10.f, 10.f);
	directLight_tf.rotation = glm::vec3(45.0f, 45.f, 0.f);
	directLight_light.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	directLight_light.Intensity = 1.0f;

	for (int i = 0; i < 10; i++) {
		Entity& block = entityManager.CreateEntity();

		tagStore.add(block.id, Tag::Enemy);


		TransformComponent& block_tf = transformStore.add(block.id);
		block_tf.position = glm::vec3(5.0f, 0.0f, 3.0f*i);

		modelLoader.load(block.id, "Model/NoodleCat/Cat.obj", false);

		ColliderComponent& block_col = colliderStore.add(block.id);
		block_col.size = glm::vec3(1, 1, 1);
		//block_col.isStatic = true;

		//MaterialComponent& block_mat = materialStore.get(block.id);
		float rx = (float)rand() / RAND_MAX;
		float rz = (float)rand() / RAND_MAX;
		float ra = (float)rand() / RAND_MAX;
		block_tf.position = glm::vec3(20.0f * rx, 0.0f, 20.0f * rz);
		block_tf.rotation = glm::vec3(0, 360.f * ra, 0);
	}

	Entity& floor = entityManager.CreateEntity();
	TransformComponent& floor_tf = transformStore.add(floor.id);

	modelLoader.load(floor.id, "Model/Floor/floor.obj", false);

	cameraSystem.Init();
}

int GetEnemyLeft() {
	Game& game = Game::getInstance();
	std::vector<uint8_t> enemies = game.tagStore.getEntities(Tag::Enemy);
	int left = 0;
	for (uint8_t e : enemies) {
		if (!game.entityManager.entities[e].isDestroy) left++;
	}
	return left;
}


void Game::Update(float dt)
{
	transformSystem.Update();	
	cameraSystem.Update(dt);
	inputSystem.Update(dt);
	gunSystem.Update(dt);
	colliderSystem.Update();
	rendererSystem.Render();

	// 2. then ImGui on top
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// your UI here
	ImGui::Begin("HUD");
	int enemyLeft = GetEnemyLeft();
	if (enemyLeft > 0) {
		ImGui::Text("Enemies Left: %d", enemyLeft);
	}
	else {
		ImGui::Text("Yippe :3");
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{

}
