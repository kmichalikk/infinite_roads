#pragma once

#include "render_node.h"
#include "../texture.h"

class Ground : public RenderNode {
private:
    unsigned int VAO;
    Texture texture;
    bool blueprint = true;
    void init(int canvasSize);
public:
    explicit Ground(int canvasSize);
    void doDraw(double dt, glm::mat4 *parentTransform) override;
    void toggleBlueprint();
};
