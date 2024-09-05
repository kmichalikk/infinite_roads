#pragma once

#include "render_node.h"

class Blueprint : public RenderNode {
private:
    unsigned int VAO;
    void init(int canvasSize);
public:
    Blueprint(int canvasSize);
    void draw(double dt) override;
};
