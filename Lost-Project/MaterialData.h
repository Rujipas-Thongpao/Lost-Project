#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Texture.h"

class MaterialData {
public:
    std::vector<Texture2D> textures_loaded;
    std::string          directory;
    bool            gammaCorrection = false;
};