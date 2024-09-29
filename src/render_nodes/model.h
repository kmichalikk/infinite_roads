#pragma once
#include <map>
#include <vector>

#include "mesh.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "../texture.h"

class Model : public RenderNode {
private:
    std::string shaderName;
    std::map<std::string, Texture> loadedTextures;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, glm::vec3 offsetPosition);
    std::vector<Texture> loadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::string typeName);
public:
    Model(std::string name, std::string shaderName, const char *path);
};
