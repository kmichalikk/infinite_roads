#pragma once

#include "render_node.h"
#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL

class Car : public RenderNode {
private:
    glm::vec3 position;
    glm::vec3 normal;
    float tangentAngle;
    float normalAngle;
    unsigned int VAO;

    void calculateAngles();
public:
    Car();
    void draw(double dt) override;
    void setPosition(glm::vec3 position) { this->position = position; };
    void setNormal(glm::vec3 normal);
};
