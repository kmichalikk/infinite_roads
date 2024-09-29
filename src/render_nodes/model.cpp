#include "model.h"

#include <stb_image.h>
#include <Eigen/Core>

#include "../macros.h"
#include "assimp/postprocess.h"
#include "../texture.h"

Model::Model(std::string name, std::string shaderName, const char *path)
    : RenderNode(name, shaderName), shaderName(shaderName) {
    loadModel(path);
}


void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        ERROR("ASSIMP", importer.GetErrorString());
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        // assuming model nodes are one level deep, todo: combine positions from all parent nodes
        glm::vec3 offsetPosition(node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c4);
        addChild(std::make_shared<Mesh>(processMesh(mesh, scene, offsetPosition)));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, glm::vec3 offsetPosition) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 textureCoords;
            textureCoords.x = mesh->mTextureCoords[0][i].x;
            textureCoords.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = textureCoords;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseTextures = loadMaterialTextures(scene, material, aiTextureType_DIFFUSE, "texture_diffuse");

    Mesh finishedMesh(mesh->mName.C_Str(), shaderName, vertices, indices, diffuseTextures);
    finishedMesh.setPosition(offsetPosition);

    return finishedMesh;
}

std::vector<Texture> Model::loadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        if (loadedTextures.count(str.C_Str()) > 0) {
            continue;
        }

        const aiTexture *texture = scene->GetEmbeddedTexture(str.C_Str());
        int width, height, channelCount;
        if (texture != nullptr) {
            unsigned char *data = stbi_load_from_memory(reinterpret_cast<stbi_uc const *>(texture->pcData), texture->mWidth, &width, &height, &channelCount, 0);
            textures.push_back(Texture{width, height, data});
            stbi_image_free(data);
        }

        loadedTextures[str.C_Str()] = textures.back();
    }

    return textures;
}
