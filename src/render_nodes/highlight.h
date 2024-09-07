#pragma once
#include "render_node.h"

#include "glm/glm.hpp"

class Highlight : public RenderNode {
private:
    unsigned int VAO;
    glm::vec3 position;
    glm::vec3 color;
public:
    Highlight(glm::vec3 color);
    void setPosition(glm::vec3 position) { this->position = position; }
    void draw(double dt) override;
};
