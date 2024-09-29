#include "highlight.h"

#include <vector>
#include <cmath>
#include <glad/glad.h>

#include "../resource_manager.h"
#include "glm/ext/matrix_transform.hpp"

Highlight::Highlight(glm::vec3 color, std::string name)
    : RenderNode(name, "highlight"), color(color) {
    float points[288];
    float theta = 2 * M_PI / 12;
    for (int i = 0; i < 12; i++) {
        int k = i * 24;

        points[k] = cos(theta * (i+1));
        points[k+1] = 0.0;
        points[k+2] = sin(theta * (i+1));
        points[k+3] = 1.0;

        points[k+4] = cos(theta * i);
        points[k+5] = 0.0;
        points[k+6] = sin(theta * i);
        points[k+7] = 1.0;

        points[k+8] = cos(theta * i);
        points[k+9] = 2.0;
        points[k+10] = sin(theta * i);
        points[k+11] = 0.0;

        points[k+12] = cos(theta * i);
        points[k+13] = 2.0;
        points[k+14] = sin(theta * i);
        points[k+15] = 0.0;

        points[k+16] = cos(theta * (i+1));
        points[k+17] = 2.0;
        points[k+18] = sin(theta * (i+1));
        points[k+19] = 0.0;

        points[k+20] = cos(theta * (i+1));
        points[k+21] = 0.0;
        points[k+22] = sin(theta * (i+1));
        points[k+23] = 1.0;
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void *>(nullptr));
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Highlight::doDraw(double dt, glm::mat4 *parentTransform) {
    glBindVertexArray(VAO);

    glm::mat4 model = glm::translate(*parentTransform, getPosition());
    model = glm::scale(model, glm::vec3(0.3f));
    shader.setMat4("model", model);
    shader.setVec3f("highlightColor", color);

    glDisable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 36, 36);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_CULL_FACE);

    glBindVertexArray(0);
}
