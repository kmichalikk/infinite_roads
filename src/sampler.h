#pragma once

#include <vector>

#include "interpolation_node.h"

template <typename T>
class Sampler {
private:
    std::vector<InterpolationNode<T>> samples;
    float tMax;

public:
    Sampler(std::vector<InterpolationNode<T>> samples)
        : samples(samples) {
        tMax = samples.size() > 0 ? samples.back().t : 0.0f;
    }

    float getTMax() const { return tMax; };
    int getSampleCount() const { return samples.size(); };
    InterpolationNode<T> getNthSample(int n) const { return samples[n]; };
    void setNthSample(int n, InterpolationNode<T> newSample) { samples[n] = newSample; };

    T sample(float t) {
        int leftmostIndex = bisectSamples(t);
        if (leftmostIndex+1 == samples.size()) {
            return samples[leftmostIndex].value;
        }

        float diffT = samples[leftmostIndex+1].t - samples[leftmostIndex].t;
        float lerpT = (samples[leftmostIndex+1].t - t) / diffT;

        T v1 = samples[leftmostIndex].value;
        T v2 = samples[leftmostIndex+1].value;

        return { v1 * lerpT + v2 * (1-lerpT) };
    };

private:
    int bisectSamples(float t) const {
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
    };
};
