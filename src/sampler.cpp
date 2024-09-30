#include "sampler.h"

Sampler::Sampler(std::vector<InterpolationNode<glm::vec3>> samples)
    : samples(samples) {
    tMax = samples.back().t;
}

glm::vec3 Sampler::sample(float t) {
    int leftmostIndex = bisectSamples(t);
    if (leftmostIndex+1 == samples.size()) {
        return samples[leftmostIndex].value;
    }

    float diffT = samples[leftmostIndex+1].t - samples[leftmostIndex].t;
    float lerpT = (samples[leftmostIndex+1].t - t) / diffT;

    glm::vec3 v1 = samples[leftmostIndex].value;
    glm::vec3 v2 = samples[leftmostIndex+1].value;

    return {
        v1.x * lerpT + v2.x * (1-lerpT),
        v1.y * lerpT + v2.y * (1-lerpT),
        v1.z * lerpT + v2.z * (1-lerpT)
    };
}

void Sampler::setNthSample(int n, InterpolationNode<glm::vec3> newSample) {
    samples[n] = newSample;
}

int Sampler::bisectSamples(float t) const {
    int mid, left = 0;
    int right = samples.size()-1;

    while (left < right) {
        mid = (left + right) / 2;
        if (samples[mid].t < t) {
            left = mid+1;
        } else {
            right = mid;
        }
    }

    return mid;
}

