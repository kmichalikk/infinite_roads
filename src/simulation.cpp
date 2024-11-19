#include "simulation.h"

#include <utility>

Simulation::Simulation()
    : isRunning(false), t(0.0) {
}

void Simulation::initialize(std::shared_ptr<Car> car, std::shared_ptr<Sampler<glm::vec3> > positionSampler,
                            std::shared_ptr<Sampler<glm::vec3> > normalSampler) {
    this->car = std::move(car);
    this->positionSampler = std::move(positionSampler);
    this->normalSampler = std::move(normalSampler);
}

void Simulation::update(float deltaTime) {
    t += deltaTime * 5;
    t = fmod(t, positionSampler->getTMax());
    car->setPosition(positionSampler->sample(t));
    car->setNormal(normalSampler->sample(t));
}
