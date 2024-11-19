#pragma once
#include <memory>

#include "sampler.h"
#include "render_nodes/car.h"

class Simulation {
private:
    std::shared_ptr<Car> car;
    std::shared_ptr<Sampler<glm::vec3>> positionSampler;
    std::shared_ptr<Sampler<glm::vec3>> normalSampler;
    bool isRunning;
    float t;
public:
    Simulation();
    void initialize(
        std::shared_ptr<Car> car,
        std::shared_ptr<Sampler<glm::vec3>> positionSampler,
        std::shared_ptr<Sampler<glm::vec3>> normalSampler
    );
    void update(float deltaTime);
};
