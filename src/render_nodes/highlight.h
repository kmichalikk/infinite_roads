#pragma once
#include "render_node.h"

#include "glm/glm.hpp"

class Highlight : public RenderNode {
private:
    unsigned int VAO;
    glm::vec3 position;
public:
    Highlight();
    void setPosition(glm::vec3 position) { this->position = position; }
    void draw(double dt) override;
};
