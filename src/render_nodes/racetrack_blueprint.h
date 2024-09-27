#pragma once

#include <vector>

#include "highlight.h"
#include "render_node.h"
#include "../spline_interpolation.h"

class RacetrackBlueprint : public RenderNode {
private:
    bool loopPossible = false;
    bool loopMade = false;
    std::vector<Highlight> interpolationNodes;
public:
    void addInterpolationNode(glm::vec3 position);
    void draw(double dt) override;
    void setShaderViewMatrix(glm::mat4 view) override;
    void setShaderProjectionMatrix(glm::mat4 projection) override;
    bool snapToFirst(Highlight *highlight);
    SplineInterpolation finish();
    bool finished() const { return loopMade; }
};
