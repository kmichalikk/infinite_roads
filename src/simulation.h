#pragma once
#include <memory>

#include "sampler.h"
#include "render_nodes/car.h"
#include "render_nodes/debug_vectors.h"

class Simulation {
private:
    std::shared_ptr<Car> car;
    std::shared_ptr<Sampler<glm::vec3>> positionSampler;
    std::shared_ptr<Sampler<glm::vec3>> normalSampler;
    std::shared_ptr<Sampler<glm::vec3>> tangentSampler;
    std::shared_ptr<Sampler<glm::vec3>> accelerationSampler;
    bool isRunning;
    float t;

    Sampler<float> getSmoothedOutCurvatureAtT() const;
    static Sampler<float> makePrefixSums(Sampler<float> floatSampler);
    static Sampler<float> makeScaledLUT(Sampler<float> prefixSumSampler, float scale);
    static Sampler<float> inverseLUT(Sampler<float> lut);
    void applyLUTToSamplers(Sampler<float> LUT) ;
public:
    Simulation();
    void initialize(
        std::shared_ptr<Car> car,
        std::shared_ptr<Sampler<glm::vec3>> positionSampler,
        std::shared_ptr<Sampler<glm::vec3>> normalSampler,
        std::shared_ptr<Sampler<glm::vec3>> tangentSampler,
        std::shared_ptr<Sampler<glm::vec3>> accelerationSampler
    );
    void update(float deltaTime);
};
