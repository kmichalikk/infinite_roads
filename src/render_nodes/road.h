#pragma once

#include "render_node.h"
#include "../spline_interpolation.h"

class Road : public RenderNode {
private:
    static const int SAMPLE_SIZE = 1000;
    unsigned int VAO;
    float vertexData[SAMPLE_SIZE * 18];
    glm::vec3 lastLeft, lastRight;
    bool sampled = false;
public:
    Road();
    void sampleFrom(SplineInterpolation &interpolation);
    void draw(double dt) override;
};
