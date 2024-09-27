#include "render_node.h"

void RenderNode::prepare() {
    shader.use();
}

void RenderNode::setShaderViewMatrix(glm::mat4 view) {
    shader.setMat4("view", view);
}

void RenderNode::setShaderProjectionMatrix(glm::mat4 projection) {
    shader.setMat4("projection", projection);
}
