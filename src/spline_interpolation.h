#pragma once
#include <vector>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

template<typename T>
struct ParameterSpaceValue {
    float t;
    T value;
};

struct QubicCoeff {
    float a, b, c, d;
    QubicCoeff(const float a, const float b, const float c, const float d)
        : a(a), b(b), c(c), d(d) {  }
    QubicCoeff derivative() const { return { 3 * a, 2 * b, 1, 0 }; }
};

class SplineInterpolation {
private:
    static const float SCALE;
    std::vector<ParameterSpaceValue<glm::vec3>> samples;
    std::vector<ParameterSpaceValue<glm::vec3>> normalSamples;

    static std::vector<float> calculateChordalLength(const std::vector<glm::vec3> &nodes);
    static std::vector<ParameterSpaceValue<float[3]>> chordalLenghtToParameterSpace(const std::vector<glm::vec3> &nodes, std::vector<float> chords);
    static std::vector<ParameterSpaceValue<QubicCoeff>> solveAxis(std::vector<ParameterSpaceValue<float[3]>> parameterSpaceValues, int axis);
    int bisectSamples(float t) const;
    glm::vec3 sample(float t, bool normal);
public:
    SplineInterpolation();
    explicit SplineInterpolation(const std::vector<glm::vec3> &nodes);
    glm::vec3 samplePosition(float t);
    glm::vec3 sampleNormal(float t);
};
