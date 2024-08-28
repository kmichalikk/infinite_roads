#include "render_node.h"

void RenderNode::setShaderViewMatrix(glm::mat4 view) const {
    shader.setMat4("view", view);
}

void RenderNode::setShaderProjectionMatrix(glm::mat4 projection) const {
    shader.setMat4("projection", projection);
}
