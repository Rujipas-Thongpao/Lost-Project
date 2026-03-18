#pragma once
#include <stdio.h>

#include <vector>


#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include "Texture.h"
#include "Entity.h"

#include "MaterialData.h"
#include "MeshData.h"

class ModelLoader {
public:
    uint8_t id;
    std::vector<MeshData>  meshDatas;
    std::vector<MaterialData> materialDatas;

    uint8_t load(const char* path, bool gammaCorrection = false);

private:
    void loadFromFile(const char* path, MeshData& mc, MaterialData& mat, bool gammaCorrection);
    void processNode(aiNode* node, const aiScene* scene, MeshData& mc, MaterialData& mat);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, MaterialData& mat);
    vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, MaterialData& matComp);
    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);
};
