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

#include "ModelData.h"


/// <summary>
/// This class load and store MeshData(Model) and Material
/// </summary>
class ModelLoader {
public:
    uint8_t id;
    //std::vector<MeshData>  meshDatas;
    //std::vector<MaterialData> materialDatas;
    //std::vector<BoneData> boneDatas;

    std::vector<ModelData> modelDatas;

    uint8_t load(const char* path, bool gammaCorrection = false);

private:
    void loadFromFile(const char* path, ModelData& model, bool gammaCorrection);
    void processNode(aiNode* node, const aiScene* scene, ModelData& model);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, ModelData& model);
    vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, MaterialData& matComp);
    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);

    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene, ModelData& model);
};
