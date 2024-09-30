#pragma once

#include <vector>

#include "interpolation_node.h"
#include "glm/glm.hpp"

class Sampler {
private:
    std::vector<InterpolationNode<glm::vec3>> samples;
    float tMax;
    int bisectSamples(float t) const;
public:
    Sampler(std::vector<InterpolationNode<glm::vec3>> samples);

    glm::vec3 sample(float t);
    float getTMax() const { return tMax; };

    int getSampleCount() const { return samples.size(); };
    InterpolationNode<glm::vec3> getNthSample(int n) const { return samples[n]; };
    void setNthSample(int n, InterpolationNode<glm::vec3> newSample);
};
