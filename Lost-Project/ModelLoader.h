#pragma once
#pragma once
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include "Texture.h"
#include "Entity.h";

class ModelLoader {
public:
    static uint8_t load(Entity e, const char* path, bool gammaCorrection = false);

private:
    static void loadFromFile(const char* path, MeshComponent& mc, MaterialComponent& mat, bool gammaCorrection);
    static void processNode(aiNode* node, const aiScene* scene, MeshComponent& mc, MaterialComponent& mat);
    static Mesh processMesh(aiMesh* mesh, const aiScene* scene, MaterialComponent& mat);
    static vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, MaterialComponent& matComp);
    static unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);
};
