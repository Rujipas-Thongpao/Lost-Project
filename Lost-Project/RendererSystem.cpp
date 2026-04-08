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
	shader = ResourceManager::LoadShader("vertex.vs", "fragment.vs", nullptr, "Default");
	spriteShader = ResourceManager::LoadShader("vertex_sprite.vs", "fragment_sprite.vs", nullptr, "Default");
	depthOnlyShader = ResourceManager::LoadShader("vertex_depthOnly.vs", "fragment_depthOnly.vs", nullptr, "DepthOnly");

	// TODO : Shadow buffer shouldn't contain color texture, only depth texture. Also need to set up light's view and projection matrices for rendering to shadow buffer.
	shadowBuffer.Init(800, 800);
	postProcessSystem.Init(800, 800);

}

void RendererSystem::RenderScene(Shader& shader, Shader& spriteShader) {
	glEnable(GL_DEPTH_TEST);

	glClearColor(153.f/255.f, 213.f/255.f, 226.f/255.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Game& game = Game::getInstance();
	std::pair<uint8_t, CameraComponent&> cam_e_c = game.cameraStore.getFirst();
	CameraComponent& cam_cam = cam_e_c.second;
	uint16_t cam_id = cam_e_c.first;
	TransformComponent& cam_tf = game.transformStore.get(cam_id);
	//GLMUtils::printVec3(cam_tf.position, "Camera position : ");
	
	for (Entity& e : game.entityManager.entities) {
		//std::cout << (unsigned)e.id <<  " " << e.isDestroy << std::endl;
		if (e.isDestroy) continue;
		if (!game.meshStore.has(e.id) && !game.spriteStore.has(e.id))      continue;
		if (!game.transformStore.has(e.id)) continue;

		 //std::cout << "Render : " << (unsigned)e.id << std::endl;
		TransformComponent& transform = game.transformStore.get(e.id);
		MaterialComponent& mat = game.materialStore.get(e.id);

		// Select Shader to use whether it's mesh or sprite
		Shader renderShader;
		if (game.meshStore.has(e.id)) {
			renderShader = shader;
		}
		else if (game.spriteStore.has(e.id)) {
			renderShader = spriteShader;
		}

		renderShader.Use();
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, transform.position);

		model = glm::rotate(model, glm::radians(-transform.rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));

		model = glm::scale(model, transform.scale);

		glm::mat4 view = cam_cam.GetViewMatrix(cam_tf);
		glm::mat4 proj = cam_cam.GetProjectionMatrix();

		renderShader.SetMatrix4("model", model);
		renderShader.SetMatrix4("view", cam_cam.GetViewMatrix(cam_tf));
		renderShader.SetMatrix4("projection", cam_cam.GetProjectionMatrix());

		renderShader.SetVector3f("material.ambient", mat.ambient);
		renderShader.SetVector3f("material.diffuse", mat.diffuse);
		renderShader.SetVector3f("material.specular", mat.specular);
		renderShader.SetFloat("material.shininess", mat.shininess);

		// light
		std::pair<uint16_t, LightComponent> light_id_l = game.lightStore.getFirst();
		uint16_t light_id = light_id_l.first;
		LightComponent light_l = light_id_l.second;
		TransformComponent light_tf = game.transformStore.get(light_id);

		float near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		renderShader.SetVector3f("lightPosition", light_tf.position);
		renderShader.SetVector3f("lightColor", light_l.Color);

		glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		depthOnlyShader.Use();
		depthOnlyShader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

		if (game.animationStore.has(e.id)) {
			renderShader.SetInteger("isAnimated", 1);
			AnimationComponent& anim = game.animationStore.get(e.id);
			for (int i = 0; i < 100; i++)
				renderShader.SetMatrix4("finalBonesMatrices[" + std::to_string(i) + "]",
					anim.finalBoneMatrices[i]);
		}
		else {
			renderShader.SetInteger("isAnimated", 0);
			for (int i = 0; i < 100; i++)
				renderShader.SetMatrix4("finalBonesMatrices[" + std::to_string(i) + "]",
					glm::mat4(1.0f));
		}

		if (game.meshStore.has(e.id)) {
			MeshComponent& mesh = game.meshStore.get(e.id);
			MeshData& meshData = game.modelLoader.modelDatas[mesh.mesh_id].mc;
			for (unsigned int i = 0; i < meshData.meshes.size(); i++) {
				//std::cout << mesh.meshes[i].vertices.size() << std::endl;
				meshData.meshes[i].Draw(shader);
			}
		}

		if (game.spriteStore.has(e.id)) {
			SpriteComponent& sprite = game.spriteStore.get(e.id);
			MeshData& meshData = game.modelLoader.modelDatas[game.assetManager.GetModelData("Quad")].mc;
			for (unsigned int i = 0; i < meshData.meshes.size(); i++) {
				//std::cout << mesh.meshes[i].vertices.size() << std::endl;
				meshData.meshes[i].DrawSprite(spriteShader, ResourceManager::GetTexture(sprite.textureName));
			}
		}
	}

}

void RendererSystem::RenderParticles() {
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	Game& game = Game::getInstance();
	// get quad mesh — same quad you use for sprites
	MeshData& quadMesh = game.modelLoader.modelDatas[
		game.assetManager.GetModelData("Quad")
	].mc;

	// loop entities that have ParticleComponent
	for (Entity& e : game.entityManager.entities) {
		if (e.isDestroy) continue;
		if (!game.particleStore.has(e.id)) continue;

		ParticleComponent& pc = game.particleStore.get(e.id);

		uint16_t cam = game.tagStore.getEntity(Tag::Camera);
		CameraComponent& cam_cam = game.cameraStore.get(cam);
		TransformComponent& cam_tf = game.transformStore.get(cam);

		glm::mat4 view = cam_cam.GetViewMatrix(cam_tf);
		glm::mat4 proj = cam_cam.GetProjectionMatrix();

		// loop each active particle inside the component
		for (auto& p : pc.particles) {
			if (!p.active) continue;

			// build model matrix for this particle
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, p.position);
			model = glm::scale(model, glm::vec3(p.size, p.size, 1.0f));

			// set uniforms
			spriteShader.Use();
			spriteShader.SetMatrix4("model", model);
			spriteShader.SetMatrix4("view", view);
			spriteShader.SetMatrix4("projection", proj);
			spriteShader.SetVector4f("color", p.color);
			spriteShader.SetFloat("size", p.size);

			// reuse same quad mesh
			for (auto& mesh : quadMesh.meshes)
				mesh.DrawSprite(spriteShader,
					ResourceManager::GetTexture(pc.sprite));
		}
	}

	// restore state
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RendererSystem::Render()
{
	glViewport(0, 0, 800, 800);
	shadowBuffer.Bind();
	shadowBuffer.Clear();
	RenderScene(depthOnlyShader, depthOnlyShader);
	//shadowBuffer.Unbind();

	//postProcessSystem.BeginScene();
	//glBindTexture(GL_TEXTURE_2D, shadowBuffer.depthTexture);

	//RenderScene(shader, spriteShader);
	//postProcessSystem.EndScene();

	//postProcessSystem.Render();
}