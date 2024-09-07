#pragma once

#include "glm/glm.hpp"
#include "../shader.h"

class RenderNode {
protected:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    Shader shader;
public:
    virtual ~RenderNode() = default;
    virtual void draw(double dt) = 0;
    virtual void prepare();
    virtual void setShaderViewMatrix(glm::mat4 view) const;
    virtual void setShaderProjectionMatrix(glm::mat4 projection) const;
};
