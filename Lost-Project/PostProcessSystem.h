#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "framebuffer.h"
#include "ScreenQuad.h"
#include "Shader.h"
#include "ResourceManager.h"

// PostProcessSystem.h
class PostProcessSystem {
    Framebuffer  sceneBuffer;   // scene renders here
    Framebuffer  pingBuffer;    // ping pong for chaining
    Framebuffer  pongBuffer;
    ScreenQuad   quad;

    Shader outlineShader;
    Shader ssaoShader;
    Shader passthroughShader;   // just outputs texture as-is

    bool enableOutline = true;
    bool enableSSAO = false;

public:
    void Init(int w, int h) {
        sceneBuffer.Init(w, h);
        pingBuffer.Init(w, h);
        pongBuffer.Init(w, h);
        quad.Init();

        outlineShader = ResourceManager::LoadShader("vertex_screen_quad.vs",
			"outline.vs", nullptr, "Outline");
        passthroughShader = ResourceManager::LoadShader("vertex_screen_quad.vs",
			"passthrough.vs", nullptr, "Passthrough");
		//ssaoShader = ResourceManager::LoadShader("vertex_screen_quad.vs", "ssao.vs", nullptr, "SSAO");
    }

    void BeginScene() {
        sceneBuffer.Bind();
        sceneBuffer.Clear();
    }

    void EndScene() {
        sceneBuffer.Unbind();
    }

    void Render() {
        glDisable(GL_DEPTH_TEST);

        unsigned int inputTex = sceneBuffer.colorTexture;

        if (enableOutline) {
            pingBuffer.Bind();
            pingBuffer.Clear();
            outlineShader.Use();
            outlineShader.SetInteger("screenTexture", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, inputTex);
            quad.Draw();
            pingBuffer.Unbind();
            inputTex = pingBuffer.colorTexture;  
        }

        //if (enableSSAO) {
        //    pongBuffer.Bind();
        //    pongBuffer.Clear();
        //    ssaoShader.Use();
        //    ssaoShader.SetInteger("screenTexture", 0);
        //    glActiveTexture(GL_TEXTURE0);
        //    glBindTexture(GL_TEXTURE_2D, inputTex);
        //    quad.Draw();
        //    pongBuffer.Unbind();
        //    inputTex = pongBuffer.colorTexture;
        //}

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        passthroughShader.Use();
        passthroughShader.SetInteger("screenTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTex);
        quad.Draw();

        glEnable(GL_DEPTH_TEST);
    }
};
