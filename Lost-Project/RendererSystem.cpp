#include "RendererSystem.h"
#include "Game.h"

#include <stdio.h>
#include <iostream>

#include "Entity.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Shader.h"
#include "CameraComponent.h"
#include "GLMUtils.h"


#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <iostream>


void RendererSystem::Init() {
	shader = ResourceManager::LoadShader("vertex.vs", "fragment.fs", nullptr, "Default");
}

void RendererSystem::Render()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Game& game = Game::getInstance();
	std::pair<uint8_t, CameraComponent&> cam_e_c = game.cameraStore.getFirst();
	CameraComponent cam_cam = cam_e_c.second;
	uint8_t cam_id = cam_e_c.first;
	TransformComponent cam_tf = game.transformStore.get(cam_id);
	GLMUtils::printVec3(cam_tf.position, "Camera position : ");
	
	for (Entity e : game.entityManager.entities) {
		if (!game.meshStore.has(e.id))      continue;
		if (!game.transformStore.has(e.id)) continue;

		// std::cout << "Render : " << (unsigned)e.id << std::endl;
		TransformComponent transform = game.transformStore.get(e.id);
		MeshComponent mesh = game.meshStore.get(e.id);

		this->shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.position);
		model = glm::scale(model, transform.scale);

		glm::mat4 view = cam_cam.GetViewMatrix(cam_tf);
		glm::mat4 proj = cam_cam.GetProjectionMatrix();

		//GLMUtils::printMat4(model, "model : ");
		//GLMUtils::printMat4(view, "view : ");
		//GLMUtils::printMat4(proj, "projection : ");

		this->shader.SetMatrix4("model", model);
		this->shader.SetMatrix4("view", cam_cam.GetViewMatrix(cam_tf));
		this->shader.SetMatrix4("projection", cam_cam.GetProjectionMatrix());

		for (unsigned int i = 0; i < mesh.meshes.size(); i++) {
			//std::cout << mesh.meshes[i].vertices.size() << std::endl;
			mesh.meshes[i].Draw(this->shader);
		}
	}
}