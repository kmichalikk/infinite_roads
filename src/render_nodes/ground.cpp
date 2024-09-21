#include "ground.h"

#include <glad/glad.h>

#include "../resource_manager.h"

Ground::Ground(int canvasSize) {
    shader = ResourceManager::getShader("blueprint");
    texture = ResourceManager::getTexture("grass");
    init(canvasSize);
}

void Ground::init(int canvasSize) {
    float halfExtend = canvasSize / 2.0;
    float texSize = canvasSize * 0.3;
    float vertexData[] = {
        halfExtend, 0.0f, halfExtend, texSize, texSize,
        -halfExtend, 0.0f, -halfExtend, 0.0f, 0.0f,
        -halfExtend, 0.0f, halfExtend, 0.0f, texSize,
        -halfExtend, 0.0f, -halfExtend, 0.0f, 0.0f,
        halfExtend, 0.0f, halfExtend, texSize, texSize,
        halfExtend, 0.0f, -halfExtend, texSize, 0.0f,
    };

    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Ground::draw(double dt) {
    glBindVertexArray(VAO);

    texture.bind();

    glm::mat4 model(1.0f);
    shader.setMat4("model", model);
    shader.setInt("blueprint", blueprint);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Ground::toggleBlueprint() {
    blueprint = false;
}
