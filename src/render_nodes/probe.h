#pragma once
#include "glm/glm.hpp"
#include "render_node.h"


class Probe: public RenderNode {
private:
    glm::vec3 position;
    unsigned int VAO;
    void init();
    void drawWorldDirections(glm::vec3 direction);
public:
    Probe(glm::vec3 position);
    void draw(double dt) override;
};
