#include "blueprint.h"

#include <glad/glad.h>

#include "../resource_manager.h"

Blueprint::Blueprint(int canvasSize) {
    shader = ResourceManager::getShader("blueprint");
    init(canvasSize);
}

void Blueprint::init(int canvasSize) {
    float halfExtend = canvasSize / 2.0;
    float vertexData[] = {
        halfExtend, 0.0f, halfExtend,
        -halfExtend, 0.0f, -halfExtend,
        -halfExtend, 0.0f, halfExtend,
        -halfExtend, 0.0f, -halfExtend,
        halfExtend, 0.0f, halfExtend,
        halfExtend, 0.0f, -halfExtend
    };

    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Blueprint::draw(double dt) {
    glBindVertexArray(VAO);

    glm::mat4 model(1.0f);
    shader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
