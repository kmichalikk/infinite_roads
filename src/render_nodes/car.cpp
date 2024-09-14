#include "car.h"

#include <iostream>
#include <glad/glad.h>

#include "../resource_manager.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"

Car::Car()
{
    shader = ResourceManager::getShader("color");

    float vertexData[] = {
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Car::calculateAngles() {
    glm::vec2 xUnit(1.0f, 0.0f);
    glm::vec2 flatNormal = glm::normalize(glm::vec2(normal.x, -normal.z));
    normalAngle = glm::orientedAngle(xUnit, flatNormal);
    tangentAngle = normalAngle - M_PI / 2.0f;
}

void Car::setNormal(glm::vec3 normal) {
    this->normal = normal;
    calculateAngles();
}

void Car::draw(double dt) {
    glBindVertexArray(VAO);

    shader.setVec3f("baseColor", glm::vec3(1, 0.376, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::rotate(model, tangentAngle, glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(0.8, 0.5, 0.5));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
