#include "probe.h"

#include <glad/glad.h>
#include "glm/glm.hpp"

#include "../resource_manager.h"
#include "glm/ext/matrix_transform.hpp"

Probe::Probe(glm::vec3 position)
    : position(position) {
    shader = ResourceManager::getShader("probe");
    init();
}

void Probe::init() {
    float vertexData[] = {
         0.5f, -0.5f, -0.5f, 0, 0, 1,
        -0.5f, -0.5f, -0.5f, 0, 0, 1,
         0.5f,  0.5f, -0.5f, 0, 0, 1,
        -0.5f,  0.5f, -0.5f, 0, 0, 1,
         0.5f,  0.5f, -0.5f, 0, 0, 1,
        -0.5f, -0.5f, -0.5f, 0, 0, 1,

        -0.5f, -0.5f,  0.5f, 0, 0, 1,
         0.5f, -0.5f,  0.5f, 0, 0, 1,
         0.5f,  0.5f,  0.5f, 0, 0, 1,
         0.5f,  0.5f,  0.5f, 0, 0, 1,
        -0.5f,  0.5f,  0.5f, 0, 0, 1,
        -0.5f, -0.5f,  0.5f, 0, 0, 1,

        -0.5f,  0.5f,  0.5f, 1, 0, 0,
        -0.5f,  0.5f, -0.5f, 1, 0, 0,
        -0.5f, -0.5f, -0.5f, 1, 0, 0,
        -0.5f, -0.5f, -0.5f, 1, 0, 0,
        -0.5f, -0.5f,  0.5f, 1, 0, 0,
        -0.5f,  0.5f,  0.5f, 1, 0, 0,

         0.5f,  0.5f, -0.5f, 1, 0, 0,
         0.5f,  0.5f,  0.5f, 1, 0, 0,
         0.5f, -0.5f, -0.5f, 1, 0, 0,
         0.5f, -0.5f,  0.5f, 1, 0, 0,
         0.5f, -0.5f, -0.5f, 1, 0, 0,
         0.5f,  0.5f,  0.5f, 1, 0, 0,

        -0.5f, -0.5f, -0.5f, 0, 1, 0,
         0.5f, -0.5f, -0.5f, 0, 1, 0,
         0.5f, -0.5f,  0.5f, 0, 1, 0,
         0.5f, -0.5f,  0.5f, 0, 1, 0,
        -0.5f, -0.5f,  0.5f, 0, 1, 0,
        -0.5f, -0.5f, -0.5f, 0, 1, 0,

         0.5f,  0.5f, -0.5f, 0, 1, 0,
        -0.5f,  0.5f, -0.5f, 0, 1, 0,
         0.5f,  0.5f,  0.5f, 0, 1, 0,
        -0.5f,  0.5f,  0.5f, 0, 1, 0,
         0.5f,  0.5f,  0.5f, 0, 1, 0,
        -0.5f,  0.5f, -0.5f, 0, 1, 0
    };

    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Probe::drawWorldDirections(glm::vec3 direction) {
    glm::vec3 scale = 0.5f * direction + glm::vec3(0.08f);
    glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), position + 0.4f * direction), scale);
    shader.setVec3f("overrideColor", direction);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Probe::draw(double dt) {
    glBindVertexArray(VAO);

    glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(0.3f));
    shader.setMat4("model", model);
    shader.setInt("skipNormal", 0);
    shader.setVec3f("overrideColor", glm::vec3(0.0));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader.setInt("skipNormal", 1);

    drawWorldDirections(glm::vec3(1, 0, 0));
    drawWorldDirections(glm::vec3(0, 1, 0));
    drawWorldDirections(glm::vec3(0, 0, 1));

    glBindVertexArray(0);
}
