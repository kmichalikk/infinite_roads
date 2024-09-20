#include "road.h"

#include <glad/glad.h>
#include "glm/glm.hpp"

#include "../macros.h"
#include "../resource_manager.h"
#include "glm/ext/matrix_transform.hpp"

Road::Road() {
    shader = ResourceManager::getShader("color");
}

void Road::sampleFrom(SplineInterpolation &interpolation) {
    int i = 0;
    float step = 1.0f / SAMPLE_SIZE;
    float t = 0.0f;
    glm::vec3 currentCenter = interpolation.samplePosition(t);
    glm::vec3 currentNormal = interpolation.sampleNormal(t) * 0.5f;
    lastRight = currentCenter + currentNormal;
    lastLeft = currentCenter - currentNormal;
    glm::vec3 currentRight, currentLeft;
    t += step;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        int k = i * 18;
        currentCenter = interpolation.samplePosition(t);
        currentNormal = interpolation.sampleNormal(t) * 0.5f;
        currentLeft = currentCenter + currentNormal;
        currentRight = currentCenter - currentNormal;

        vertexData[k] = lastRight.x;
        vertexData[k+1] = lastRight.y;
        vertexData[k+2] = lastRight.z;

        vertexData[k+3] = lastLeft.x;
        vertexData[k+4] = lastLeft.y;
        vertexData[k+5] = lastLeft.z;

        vertexData[k+6] = currentRight.x;
        vertexData[k+7] = currentRight.y;
        vertexData[k+8] = currentRight.z;

        vertexData[k+9] = currentRight.x;
        vertexData[k+10] = currentRight.y;
        vertexData[k+11] = currentRight.z;

        vertexData[k+12] = currentLeft.x;
        vertexData[k+13] = currentLeft.y;
        vertexData[k+14] = currentLeft.z;

        vertexData[k+15] = lastRight.x;
        vertexData[k+16] = lastRight.y;
        vertexData[k+17] = lastRight.z;

        lastRight = currentLeft;
        lastLeft = currentRight;
        t += step;
    }

    int size = SAMPLE_SIZE * 18;

    vertexData[size-18] = vertexData[size-21];
    vertexData[size-17] = vertexData[size-20];
    vertexData[size-16] = vertexData[size-19];

    vertexData[size-15] = vertexData[size-24];
    vertexData[size-14] = vertexData[size-23];
    vertexData[size-13] = vertexData[size-22];

    vertexData[size-12] = vertexData[3];
    vertexData[size-11] = vertexData[4];
    vertexData[size-10] = vertexData[5];

    vertexData[size-9] = vertexData[3];
    vertexData[size-8] = vertexData[4];
    vertexData[size-7] = vertexData[5];

    vertexData[size-6] = vertexData[0];
    vertexData[size-5] = vertexData[1];
    vertexData[size-4] = vertexData[2];

    vertexData[size-3] = vertexData[size-21];
    vertexData[size-2] = vertexData[size-20];
    vertexData[size-1] = vertexData[size-19];

    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(vertexData[0]), static_cast<void *>(nullptr));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sampled = true;
}

void Road::draw(double dt) {
    if (!sampled) {
        ERROR("ROAD", "No vertex data for draw() call");
        return;
    }

    glBindVertexArray(VAO);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.01f, 0.0f));
    shader.setMat4("model", model);
    shader.setVec3f("baseColor", glm::vec3(0.6f));
    glDrawArrays(GL_TRIANGLES, 0, SAMPLE_SIZE * 6);
    glBindVertexArray(0);
}
