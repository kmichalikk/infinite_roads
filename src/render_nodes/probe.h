#pragma once
#include "glm/glm.hpp"
#include "render_node.h"


class Probe: public RenderNode {
private:
    unsigned int VAO;
    void init();
    void drawWorldDirections(glm::vec3 direction, glm::mat4 *parentTransform);
public:
    explicit Probe(glm::vec3 position);
    void doDraw(double dt, glm::mat4 *parentTransform) override;
};
