#include "RendererSystem.h"
#include "Game.h"

#include <stdio.h>
#include <iostream>

#include "Entity.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "CameraComponent.h"
#include "GLMUtils.h"
#include "LightComponent.h"
#include "ModelLoader.h";

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


void RendererSystem::Init() {
	shader = ResourceManager::LoadShader("vertex.vs", "fragment.fs", nullptr, "Default");
}

void RendererSystem::Render()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(153.f/255.f, 213.f/255.f, 226.f/255.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Game& game = Game::getInstance();
	std::pair<uint8_t, CameraComponent&> cam_e_c = game.cameraStore.getFirst();
	CameraComponent& cam_cam = cam_e_c.second;
	uint16_t cam_id = cam_e_c.first;
	TransformComponent& cam_tf = game.transformStore.get(cam_id);
	//GLMUtils::printVec3(cam_tf.position, "Camera position : ");
	
	for (Entity& e : game.entityManager.entities) {
		//std::cout << (unsigned)e.id <<  " " << e.isDestroy << std::endl;
		if (e.isDestroy) continue;
		if (!game.meshStore.has(e.id))      continue;
		if (!game.transformStore.has(e.id)) continue;

		 //std::cout << "Render : " << (unsigned)e.id << std::endl;
		TransformComponent& transform = game.transformStore.get(e.id);
		MeshComponent& mesh = game.meshStore.get(e.id);
		MaterialComponent& mat = game.materialStore.get(e.id);

		this->shader.Use();
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, transform.position);

		model = glm::rotate(model, glm::radians(-transform.rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));

		model = glm::scale(model, transform.scale);

		glm::mat4 view = cam_cam.GetViewMatrix(cam_tf);
		glm::mat4 proj = cam_cam.GetProjectionMatrix();

		this->shader.SetMatrix4("model", model);
		this->shader.SetMatrix4("view", cam_cam.GetViewMatrix(cam_tf));
		this->shader.SetMatrix4("projection", cam_cam.GetProjectionMatrix());

		this->shader.SetVector3f("material.ambient", mat.ambient);
		this->shader.SetVector3f("material.diffuse", mat.diffuse);
		this->shader.SetVector3f("material.specular", mat.specular);
		this->shader.SetFloat("material.shininess", mat.shininess);

		// light
		std::pair<uint16_t, LightComponent> light_id_l = game.lightStore.getFirst();
		uint16_t light_id = light_id_l.first;
		LightComponent light_l = light_id_l.second;
		TransformComponent light_tf = game.transformStore.get(light_id);

		this->shader.SetVector3f("lightPosition", light_tf.position);
		this->shader.SetVector3f("lightColor", light_l.Color);


		// TODO : Set animation here.
		// get animator here

				// check if animated
		if (game.animationStore.has(e.id)) {
			AnimationComponent& anim = game.animationStore.get(e.id);

			// upload all 100 bone matrices to shader
			for (int i = 0; i < 100; i++) {
				//std::string uniform = "finalBonesMatrices[" + std::to_string(i) + "]";
				shader.SetMatrix4("finalBonesMatrices[" + std::to_string(i) + "]", anim.finalBoneMatrices[i]);
			}
		}


		MeshData& meshData = game.modelLoader.modelDatas[mesh.mesh_id].mc;
		for (unsigned int i = 0; i < meshData.meshes.size(); i++) {
			//std::cout << mesh.meshes[i].vertices.size() << std::endl;
			meshData.meshes[i].Draw(this->shader);
		}
	}
}