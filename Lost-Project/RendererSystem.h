#pragma once
#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H

#include "Shader.h"

class RendererSystem {
	Shader shader;
    Shader postShader;
	unsigned int framebuffer;
    unsigned int quadVAO, quadVBO;
    unsigned int textureColorBuffer;
    float quadVertices[6*4] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
public:
	void Init();
	void Render();

};

#endif