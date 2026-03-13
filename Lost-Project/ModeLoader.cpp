#include "ModelLoader.h"
#include "Game.h"

uint8_t ModelLoader::load(uint8_t e_id, const char* path, bool gammaCorrection) {
    Game& game = Game::getInstance();

    game.meshStore.add(e_id);
    game.materialStore.add(e_id);

    MeshComponent& mc = game.meshStore.get(e_id);
    MaterialComponent& mat = game.materialStore.get(e_id);
    mat.gammaCorrection = gammaCorrection;

    loadFromFile(path, mc, mat, gammaCorrection);
    return e_id;
}

void ModelLoader::loadFromFile(const char* path, MeshComponent& mc, MaterialComponent& mat, bool gammaCorrection) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }

    mat.directory = string(path).substr(0, string(path).find_last_of('/'));
    processNode(scene->mRootNode, scene, mc, mat);
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene, MeshComponent& mc, MaterialComponent& mat) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mc.meshes.push_back(processMesh(mesh, scene, mat));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, mc, mat);
    }
}

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene, MaterialComponent& mat) {
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture2D>      textures;

    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // textures
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    auto diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", mat);
    auto specular = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", mat);
    auto normal = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", mat);
    auto height = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", mat);

    textures.insert(textures.end(), diffuse.begin(), diffuse.end());
    textures.insert(textures.end(), specular.begin(), specular.end());
    textures.insert(textures.end(), normal.begin(), normal.end());
    textures.insert(textures.end(), height.begin(), height.end());

    return Mesh(vertices, indices, textures);
}

vector<Texture2D> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, MaterialComponent& matComp) {
    vector<Texture2D> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString texturePath;
        mat->GetTexture(type, i, &texturePath);

        // check cache first
        bool skip = false;
        for (auto& loaded : matComp.textures_loaded) {
            if (strcmp(loaded.path.data(), texturePath.C_Str()) == 0) {
                textures.push_back(loaded);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture2D texture;
            texture.ID  = TextureFromFile(texturePath.C_Str(), matComp.directory, matComp.gammaCorrection);
            texture.type = typeName;
            texture.path = texturePath.C_Str();
            textures.push_back(texture);
            matComp.textures_loaded.push_back(texture); // cache it
        }
    }
    return textures;
}

unsigned int ModelLoader::TextureFromFile(const char* path, const string& directory, bool gamma) {
    string filename = string(path);
    cout << "Loading Texture: " << filename << endl;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else                        format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        cout << "Texture failed to load at path: " << path << endl;
    }

    stbi_image_free(data);
    return textureID;
}