#include "debug_vectors.h"

#include <glad/glad.h>

#include "../resource_manager.h"

DebugVectors::DebugVectors(std::vector<AnchoredVector> vectors, glm::vec3 color)
    : RenderNode("DebugVectors", "color"), color(color) {
    count = vectors.size();
    std::vector<float> vertexData;
    for (const AnchoredVector &v : vectors) {
        vertexData.push_back(v.position.x);
        vertexData.push_back(v.position.y);
        vertexData.push_back(v.position.z);
        vertexData.push_back(v.position.x + v.vector.x);
        vertexData.push_back(v.position.y + v.vector.y);
        vertexData.push_back(v.position.z + v.vector.z);
    }

    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DebugVectors::doDraw(double dt, glm::mat4 *parentTransform) {
    glBindVertexArray(VAO);
    shader.setVec3f("baseColor", color);
    shader.setMat4("model", *parentTransform);

    for (unsigned int i = 0; i < count; i++) {
        glDrawArrays(GL_LINES, i * 2, 2);
    }

    glBindVertexArray(0);
}

