#include "simulation.h"

#include <utility>

#include "render_nodes/debug_vectors.h"

Simulation::Simulation()
    : isRunning(false), t(0.0) {
}

void Simulation::initialize(std::shared_ptr<Car> car, std::shared_ptr<Sampler<glm::vec3> > positionSampler,
                            std::shared_ptr<Sampler<glm::vec3> > normalSampler,
                            std::shared_ptr<Sampler<glm::vec3> > tangentSampler,
                            std::shared_ptr<Sampler<glm::vec3> > accelerationSampler) {
    this->car = std::move(car);
    this->positionSampler = std::move(positionSampler);
    this->normalSampler = std::move(normalSampler);
    this->tangentSampler = std::move(tangentSampler);
    this->accelerationSampler = std::move(accelerationSampler);

    Sampler<float> curvatureBasedVelocity = getSmoothedOutCurvatureAtT();

    // add minimal velocity
    for (int i = 0; i < curvatureBasedVelocity.getSampleCount(); i++) {
        InterpolationNode<float> sample = curvatureBasedVelocity.getNthSample(i);
        sample.value += 1;
        curvatureBasedVelocity.setNthSample(i, sample);
    }

    Sampler<float> lut = makeScaledLUT(makePrefixSums(curvatureBasedVelocity), this->positionSampler->getTMax());

    applyLUTToSamplers(lut);
}

Sampler<float> Simulation::getSmoothedOutCurvatureAtT() const {
    // calculate initial approximation of curvature
    // add 0.2 to the divisor so straight lines don't return infinity
    std::vector<InterpolationNode<float>> curvatureAtT;
    float step = positionSampler->getTMax() / 1000;
    float t = 0.0f;
    while (t < positionSampler->getTMax()) {
        t += step;
        glm::vec3 tangent = tangentSampler->sample(t);
        glm::vec3 tangentCrossAcceleration = glm::cross(tangent, accelerationSampler->sample(t));
        float curvature = pow(glm::length(tangent), 3) / (glm::length(tangentCrossAcceleration) + 0.2);
        curvatureAtT.emplace_back(InterpolationNode<float>{t, curvature});
    }

    // smooth out and flatten by averaging out value at each point with values in some proximity
    // run for N times for better results
    int window = 61;
    int center = 30;
    for (int i = 0; i < 5; i++) {
        float runningAverage = 0.0;

        // pad the vector as though it was looping
        int oldSize = curvatureAtT.size();
        for (int i = 0; i < center; i++) {
            curvatureAtT.insert(curvatureAtT.begin(), *(curvatureAtT.rbegin()+2*i));
            curvatureAtT.emplace_back(curvatureAtT[2*i+1]);
        }

        for (int i = 0; i < window; i++) {
            runningAverage += curvatureAtT[i].value;
        }
        runningAverage /= window;

        std::vector<InterpolationNode<float>> smoothedCurvatureAtT;
        for (int i = center; i < oldSize + center; i++) {
            smoothedCurvatureAtT.emplace_back(InterpolationNode<float>{curvatureAtT[i].t, runningAverage});
            runningAverage -= curvatureAtT[i - center].value / window;
            runningAverage += curvatureAtT[i + center + 1].value / window;
        }
        curvatureAtT = smoothedCurvatureAtT;
    }

    return { curvatureAtT };
}

Sampler<float> Simulation::makePrefixSums(Sampler<float> floatSampler) {
    std::vector prefixSums = { InterpolationNode<float>{0.0, floatSampler.sample(0.0f)} };
    for (int i = 1; i < floatSampler.getSampleCount(); i++) {
        InterpolationNode<float> sample = floatSampler.getNthSample(i);
        sample.value += prefixSums.back().value;
        prefixSums.emplace_back(sample);
    }

    return { prefixSums };
}

Sampler<float> Simulation::makeScaledLUT(Sampler<float> prefixSumSampler, float scale) {
    std::vector<InterpolationNode<float>> lut;
    float maxValue = prefixSumSampler.sample(prefixSumSampler.getTMax());
    float scaleFactor = scale / maxValue;
    for (int i = 1; i < prefixSumSampler.getSampleCount(); i++) {
        InterpolationNode<float> sample = prefixSumSampler.getNthSample(i);
        sample.value *= scaleFactor;
        lut.emplace_back(sample);
    }

    return { lut };
}

Sampler<float> Simulation::inverseLUT(Sampler<float> lut) {
    std::vector<InterpolationNode<float>> samples;
    for (int i = 1; i < lut.getSampleCount(); i++) {
        InterpolationNode<float> sample = lut.getNthSample(i);
        samples.emplace_back(InterpolationNode<float>{ sample.value, sample.t });
    }

    return { samples };
}

void Simulation::applyLUTToSamplers(Sampler<float> LUT) {
    auto invLUT = inverseLUT(std::move(LUT));

    for (int i = 0; i < positionSampler->getSampleCount(); i++) {
        InterpolationNode<glm::vec3> sample = positionSampler->getNthSample(i);
        sample.t = invLUT.sample(sample.t);
        positionSampler->setNthSample(i, sample);

        sample = normalSampler->getNthSample(i);
        sample.t = invLUT.sample(sample.t);
        normalSampler->setNthSample(i, sample);

        sample = accelerationSampler->getNthSample(i);
        sample.t = invLUT.sample(sample.t);
        accelerationSampler->setNthSample(i, sample);

        sample = tangentSampler->getNthSample(i);
        sample.t = invLUT.sample(sample.t);
        tangentSampler->setNthSample(i, sample);
    }
}

void Simulation::update(float deltaTime) {
    t += deltaTime * 5;
    t = fmod(t, positionSampler->getTMax());
    car->setPosition(positionSampler->sample(t));
    car->setNormal(normalSampler->sample(t));
}
