#pragma once
#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H

#include "Shader.h"

class RendererSystem {
	Shader shader;
public:
	void Init();
	void Render();
};

#endif