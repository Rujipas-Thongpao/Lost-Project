#pragma once
#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H

#include "Shader.h"
#include "PostProcessSystem.h"

class RendererSystem {
	Shader shader;
    Shader spriteShader;
	Shader depthOnlyShader;
	Framebuffer shadowBuffer;
	PostProcessSystem postProcessSystem;
public:
	void Init();
	void Render();
	void RenderParticles();
	void RenderScene(Shader& shader, Shader& spriteShader);
};

#endif