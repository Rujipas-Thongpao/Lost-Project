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
	postShader = ResourceManager::LoadShader("vertex_post.vs", "fragment_post.vs", nullptr, "Default");
	spriteShader = ResourceManager::LoadShader("vertex_sprite.vs", "fragment_sprite.vs", nullptr, "Default");

	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	this->postShader.Use();
	this->postShader.SetInteger("screenTexture", 0);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RendererSystem::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

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

		renderShader.SetVector3f("u_camPos", cam_tf.position);
		renderShader.SetFloat("u_ditherFadeStart", 5.0f);
		renderShader.SetFloat("u_ditherFadeEnd", 1.0f);

		// light
		std::pair<uint16_t, LightComponent> light_id_l = game.lightStore.getFirst();
		uint16_t light_id = light_id_l.first;
		LightComponent light_l = light_id_l.second;
		TransformComponent light_tf = game.transformStore.get(light_id);

		renderShader.SetVector3f("lightPosition", light_tf.position);
		renderShader.SetVector3f("lightColor", light_l.Color);

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
				meshData.meshes[i].Draw(this->shader);
			}
		}

		if (game.spriteStore.has(e.id)) {
			SpriteComponent& sprite = game.spriteStore.get(e.id);
			MeshData& meshData = game.modelLoader.modelDatas[game.assetManager.GetModelData("Quad")].mc;
			for (unsigned int i = 0; i < meshData.meshes.size(); i++) {
				//std::cout << mesh.meshes[i].vertices.size() << std::endl;
				meshData.meshes[i].DrawSprite(this->spriteShader, ResourceManager::GetTexture(sprite.textureName));
			}
		}
	}

	game.world.grassSystem.Render(
		cam_cam.GetViewMatrix(cam_tf),
		cam_cam.GetProjectionMatrix(),
		(float)glfwGetTime()
	);

	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	this->postShader.Use();
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
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

	uint16_t cam = game.tagStore.getEntity(Tag::Camera);
	CameraComponent& cam_cam = game.cameraStore.get(cam);
	TransformComponent& cam_tf = game.transformStore.get(cam);

	glm::mat4 view = cam_cam.GetViewMatrix(cam_tf);
	glm::mat4 proj = cam_cam.GetProjectionMatrix();

	// loop entities that have ParticleComponent
	for (Entity& e : game.entityManager.entities) {
		if (e.isDestroy) continue;
		if (!game.particleStore.has(e.id)) continue;

		ParticleComponent& pc = game.particleStore.get(e.id);


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