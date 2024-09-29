#pragma once
#include <vector>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

template<typename T>
struct InterpolationNode {
    float t;
    T value;
};

struct QubicCoeff {
    float a, b, c, d;
    QubicCoeff(const float a, const float b, const float c, const float d)
        : a(a), b(b), c(c), d(d) {  }
    QubicCoeff derivative() const { return { 3 * a, 2 * b, c, 0 }; }
};

class SplineInterpolation {
private:
    float totalLength = 0.0f;
    std::vector<InterpolationNode<glm::vec3>> samples;
    std::vector<InterpolationNode<glm::vec3>> normalSamples;

    static std::vector<float> calculateChordalLength(const std::vector<glm::vec3> &nodes);
    std::vector<InterpolationNode<glm::vec3>> makeInterpolationNodes(const std::vector<glm::vec3> &nodes, std::vector<float> chords) const;
    static std::vector<InterpolationNode<QubicCoeff>> solveAxis(std::vector<InterpolationNode<glm::vec3>> interpolationNodes, int axis);
    int bisectSamples(float t) const;
    glm::vec3 sample(float t, bool normal);
public:
    SplineInterpolation();
    float getTotalLength() const { return totalLength; }
    explicit SplineInterpolation(const std::vector<glm::vec3> &nodesPositions);
    glm::vec3 samplePosition(float t);
    glm::vec3 sampleNormal(float t);
};
