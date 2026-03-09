#pragma once
#ifndef MATERIAL_COMPONENT_H
#define MATERIAL_COMPONENT_H

#include <stdio.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Texture.h"

class MaterialComponent
{
public:
    std::vector<Texture2D> textures_loaded;
    std::string          directory;
    bool            gammaCorrection = false;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};
#endif