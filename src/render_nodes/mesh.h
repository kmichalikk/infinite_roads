#pragma once

#include <vector>

#include "render_node.h"
#include "glm/glm.hpp"
#include "../texture.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

class Mesh : public RenderNode {
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    void setupMesh();
public:
    Mesh(std::string name, std::string shaderName, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void doDraw(double dt, glm::mat4 *parentTransform) override;
};
