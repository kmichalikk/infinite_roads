#pragma once

#include "render_node.h"
#include "glm/glm.hpp"

class Car : public RenderNode {
private:
    glm::vec3 position;
    glm::vec3 normal;
    unsigned int VAO;
public:
    Car();
    void draw(double dt) override;
    void setPosition(glm::vec3 position) { this->position = position; };
};
