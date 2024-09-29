#include "ground.h"

#include <glad/glad.h>

#include "../resource_manager.h"

Ground::Ground(int canvasSize) : RenderNode("ground", "blueprint") {
    texture = ResourceManager::getTexture("grass");
    init(canvasSize);
}

void Ground::doDraw(double dt, glm::mat4 *parentTransform) {
    glBindVertexArray(VAO);

    texture.bind();

    shader.setMat4("model", *parentTransform);
    shader.setInt("blueprint", blueprint);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
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

void Ground::toggleBlueprint() {
    blueprint = false;
}
