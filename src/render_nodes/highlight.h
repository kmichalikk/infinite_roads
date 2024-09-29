#pragma once
#include "render_node.h"

#include "glm/glm.hpp"

class Highlight : public RenderNode {
private:
    unsigned int VAO;
    glm::vec3 color;
public:
    Highlight(glm::vec3 color, std::string name);
    void setColor(glm::vec3 color) { this->color = color; }
    void doDraw(double dt, glm::mat4 *parentTransform) override;
};
