#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Mesh.h"

#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "animData.h"


#include "stb_image.h"

class Model
{
public:
    vector<Texture2D> textures_loaded;
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    std::map<string, BoneInfo> m_BoneInfoMap; 
    int m_BoneCounter = 0;

    auto& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }

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
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

    void SetVertexBoneDataToDefault(Vertex& vertex)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            vertex.m_BoneIDs[i] = -1;
            vertex.m_Weights[i] = 0.0f;
        }
    }
};
