#pragma once
#include <map>
#include <vector>

#include "mesh.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "../texture.h"

class Model : public RenderNode {
private:
    std::vector<Mesh> meshes;
    std::map<std::string, Texture> loadedTextures;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 position;
    float rotation;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, glm::vec3 offsetPosition);
    std::vector<Texture> loadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::string typeName);
public:
    Model() {};
    Model(const char *path);
    void draw(double dt) override;
    void setShaderProjectionMatrix(glm::mat4 projection) override;
    void setShaderViewMatrix(glm::mat4 view) override;
    void prepare() override;
    void setPosition(glm::vec3 position) { this->position = position; }
    void setRotation(float rotation) { this->rotation = rotation; }
};
