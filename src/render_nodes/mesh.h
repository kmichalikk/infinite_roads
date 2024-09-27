#pragma once

#include <string>
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
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 parentPosition = glm::vec3(0.0f);
    float parentRotation = 0.0f;
    void setupMesh();
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(double dt) override;
    void setPosition(glm::vec3 position) { this->position = position; };
    void setParentPosition(glm::vec3 position) { this->parentPosition = position; };
    void setParentRotation(float rotation) { this->parentRotation = rotation; };
};
