#pragma once

#include "highlight.h"
#include "render_node.h"
#include "../spline_interpolation.h"

class RacetrackBlueprint : public RenderNode {
private:
    bool loopPossible = false;
    bool loopMade = false;
    std::vector<glm::vec3> positions;
public:
    RacetrackBlueprint() : RenderNode("racetrackBlueprint", "blueprint") {};
    void addInterpolationNode(glm::vec3 position);
    bool snapToFirst(Highlight *highlight);
    SplineInterpolation finish();
    bool finished() const { return loopMade; }
};
