#pragma once

#include "render_node.h"
#include "../spline_interpolation.h"
#include "../texture.h"

class Road : public RenderNode {
private:
    static const int SAMPLE_SIZE = 1000;
    static const int FLOATS_PER_STEP = 6 * 5;
    unsigned int VAO;
    float vertexData[SAMPLE_SIZE * FLOATS_PER_STEP];
    bool sampled = false;
    Texture texture;
public:
    Road();
    void sampleFrom(SplineInterpolation &interpolation);
    void doDraw(double dt, glm::mat4 *parentTransform) override;
};
