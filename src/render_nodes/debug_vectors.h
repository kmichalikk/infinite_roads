#pragma once
#include "render_node.h"

struct AnchoredVector {
    glm::vec3 position;
    glm::vec3 vector;
};

class DebugVectors : public RenderNode {
private:
    unsigned int VAO;
    glm::vec3 color;
    int count;
public:
    DebugVectors(std::vector<AnchoredVector> vectors, glm::vec3 color);
    void doDraw(double dt, glm::mat4 *parentTransform) override;
};
