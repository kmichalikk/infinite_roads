#include "road.h"

#include <glad/glad.h>
#include "glm/glm.hpp"

#include "../macros.h"
#include "../resource_manager.h"
#include "glm/ext/matrix_transform.hpp"

Road::Road()
    : RenderNode("road") {
    texture = ResourceManager::getTexture("asphalt");
}

void Road::sampleFrom(SplineInterpolation &interpolation) {
    Sampler positionSampler = interpolation.getPositionSampler();
    Sampler normalSampler = interpolation.getNormalSampler();

    float step = interpolation.getTotalLength() / SAMPLE_SIZE;
    float t = 0.0f;
    glm::vec3 currentCenter = positionSampler.sample(t);
    glm::vec3 currentNormal = normalSampler.sample(t) * 0.5f;

    glm::vec3 lastCenter = currentCenter;
    glm::vec3 lastRight = currentCenter - currentNormal;
    glm::vec3 lastLeft = currentCenter + currentNormal;

    float distance = 0.0f;
    float stepDistance;
    glm::vec3 currentRight, currentLeft;
    t += step;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        int k = i * FLOATS_PER_STEP;
        currentCenter = positionSampler.sample(t);
        currentNormal = normalSampler.sample(t) * 0.5f;
        currentLeft = currentCenter + currentNormal;
        currentRight = currentCenter - currentNormal;

        stepDistance = glm::length(currentCenter - lastCenter);

        vertexData[k] = lastRight.x;
        vertexData[k+1] = lastRight.y;
        vertexData[k+2] = lastRight.z;
        vertexData[k+3] = 0.5f;
        vertexData[k+4] = distance;

        vertexData[k+5] = currentRight.x;
        vertexData[k+6] = currentRight.y;
        vertexData[k+7] = currentRight.z;
        vertexData[k+8] = 0.5f;
        vertexData[k+9] = distance + stepDistance;

        vertexData[k+10] = lastLeft.x;
        vertexData[k+11] = lastLeft.y;
        vertexData[k+12] = lastLeft.z;
        vertexData[k+13] = 0.0f;
        vertexData[k+14] = distance;

        vertexData[k+15] = currentRight.x;
        vertexData[k+16] = currentRight.y;
        vertexData[k+17] = currentRight.z;
        vertexData[k+18] = 0.5f;
        vertexData[k+19] = distance + stepDistance;

        vertexData[k+20] = currentLeft.x;
        vertexData[k+21] = currentLeft.y;
        vertexData[k+22] = currentLeft.z;
        vertexData[k+23] = 0.0f;
        vertexData[k+24] = distance + stepDistance;

        vertexData[k+25] = lastLeft.x;
        vertexData[k+26] = lastLeft.y;
        vertexData[k+27] = lastLeft.z;
        vertexData[k+28] = 0.0f;
        vertexData[k+29] = distance;

        lastRight = currentRight;
        lastLeft = currentLeft;
        lastCenter = currentCenter;
        distance += stepDistance;
        t += step;
    }

    int size = SAMPLE_SIZE * FLOATS_PER_STEP;

    currentCenter = positionSampler.sample(t);
    stepDistance = glm::length(currentCenter - lastCenter);

    // lastRight
    vertexData[size-30] = vertexData[size-45];
    vertexData[size-29] = vertexData[size-44];
    vertexData[size-28] = vertexData[size-43];
    vertexData[size-27] = vertexData[size-42];
    vertexData[size-26] = distance;

    // currentRight
    vertexData[size-25] = vertexData[0];
    vertexData[size-24] = vertexData[1];
    vertexData[size-23] = vertexData[2];
    vertexData[size-22] = vertexData[3];
    vertexData[size-21] = distance + stepDistance;

    vertexData[size-20] = vertexData[size-40];
    vertexData[size-19] = vertexData[size-39];
    vertexData[size-18] = vertexData[size-38];
    vertexData[size-17] = vertexData[size-37];
    vertexData[size-16] = distance;

    vertexData[size-15] = vertexData[0];
    vertexData[size-14] = vertexData[1];
    vertexData[size-13] = vertexData[2];
    vertexData[size-12] = vertexData[3];
    vertexData[size-11] = distance + stepDistance;

    vertexData[size-10] = vertexData[10];
    vertexData[size-9] = vertexData[11];
    vertexData[size-8] = vertexData[12];
    vertexData[size-7] = vertexData[13];
    vertexData[size-6] = distance + stepDistance;

    vertexData[size-5] = vertexData[size-40];
    vertexData[size-4] = vertexData[size-39];
    vertexData[size-3] = vertexData[size-38];
    vertexData[size-2] = vertexData[size-37];
    vertexData[size-1] = distance;

    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sampled = true;
}

void Road::doDraw(double dt, glm::mat4 *parentTransform) {
    if (!sampled) {
        ERROR("ROAD", "No vertex data for draw() call");
        return;
    }

    texture.bind();

    glBindVertexArray(VAO);
    glm::mat4 model = glm::translate(*parentTransform, glm::vec3(0.0f, 0.01f, 0.0f));
    shader.setMat4("model", model);
    shader.setVec3f("baseColor", glm::vec3(0.6f));
    glDrawArrays(GL_TRIANGLES, 0, SAMPLE_SIZE * 6);
    glBindVertexArray(0);
}
