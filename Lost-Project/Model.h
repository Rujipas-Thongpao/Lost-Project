#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

class Model
{
public:
    vector<Texture2D> textures_loaded;
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    Model(const char* path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader);
private:
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);
};
