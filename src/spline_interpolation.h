#pragma once
#include <vector>

#include "interpolation_node.h"
#include "sampler.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

struct QubicCoeff {
    float a, b, c, d;
    QubicCoeff(const float a, const float b, const float c, const float d)
        : a(a), b(b), c(c), d(d) {  }
    QubicCoeff derivative() const { return { 3 * a, 2 * b, c, 0 }; }
    QubicCoeff sndDerivative() const { return { 6 * a, 2 * b, 0, 0 }; }
};

class SplineInterpolation {
private:
    float totalLength = 0.0f;
    std::vector<InterpolationNode<glm::vec3>> samples;
    std::vector<InterpolationNode<glm::vec3>> unitNormalSamples;
    std::vector<InterpolationNode<glm::vec3>> normalSamples;

    static std::vector<float> calculateChordalLength(const std::vector<glm::vec3> &nodes);
    std::vector<InterpolationNode<glm::vec3>> makeInterpolationNodes(const std::vector<glm::vec3> &nodes, std::vector<float> chords) const;
    static std::vector<InterpolationNode<QubicCoeff>> solveAxis(std::vector<InterpolationNode<glm::vec3>> interpolationNodes, int axis);
public:
    SplineInterpolation();
    explicit SplineInterpolation(const std::vector<glm::vec3> &nodesPositions);
    float getTotalLength() const { return totalLength; }
    Sampler<glm::vec3> getPositionSampler() const { return { samples }; };
    Sampler<glm::vec3> getUnitNormalSampler() const { return { unitNormalSamples }; };
    Sampler<glm::vec3> getNormalSampler() const { return { normalSamples }; };
};
