#include "spline_interpolation.h"

#include <iostream>
#include <numeric>

#include "glm/geometric.hpp"
#include "Eigen/Sparse"

SplineInterpolation::SplineInterpolation(const std::vector<glm::vec3> &nodesPositions) {
    // pair positions with their arc distance from start
    // the distance becomes a parameter t for resulting parametric curve
    std::vector<float> chords = calculateChordalLength(nodesPositions);
    totalLength = std::accumulate(chords.begin(), chords.end(), 0.0f);
    std::vector nodes = makeInterpolationNodes(nodesPositions, chords);

    // solve for each axis separately
    std::vector xSplines = solveAxis(nodes, 0);
    std::vector ySplines = solveAxis(nodes, 1);
    std::vector zSplines = solveAxis(nodes, 2);

    QubicCoeff xCoeff = xSplines[0].value;
    QubicCoeff yCoeff = ySplines[0].value;
    QubicCoeff zCoeff = zSplines[0].value;

    QubicCoeff xDerivativeCoeff = xCoeff.derivative();
    QubicCoeff yDerivativeCoeff = yCoeff.derivative();
    QubicCoeff zDerivativeCoeff = zCoeff.derivative();

    // we assume that all roads are perfectly flat
    // so we can calculate normal vector from tangent vector and world up
    glm::vec3 up(0, 1, 0);

    // we pick a small step for sampling that is easy to represent in base 2
    double step = 1.0 / (2<<7);

    double u = 0.0;
    int splineIndex = 0;

    while (u <= totalLength) {
        if (splineIndex < nodes.size() - 1 && nodes[splineIndex+1].t < u) {
            splineIndex++;
            xCoeff = xSplines[splineIndex].value;
            yCoeff = ySplines[splineIndex].value;
            zCoeff = zSplines[splineIndex].value;
            xDerivativeCoeff = xCoeff.derivative();
            yDerivativeCoeff = yCoeff.derivative();
            zDerivativeCoeff = zCoeff.derivative();
        }

        glm::vec3 sample(
            xCoeff.a * pow(u, 3) + xCoeff.b * pow(u, 2) + xCoeff.c * u + xCoeff.d,
            yCoeff.a * pow(u, 3) + yCoeff.b * pow(u, 2) + yCoeff.c * u + yCoeff.d,
            zCoeff.a * pow(u, 3) + zCoeff.b * pow(u, 2) + zCoeff.c * u + zCoeff.d
        );
        samples.emplace_back(InterpolationNode<glm::vec3>{ (float) u, sample });

        glm::vec3 tangent(
            xDerivativeCoeff.a * pow(u, 2) + xDerivativeCoeff.b * u + xDerivativeCoeff.c,
            yDerivativeCoeff.a * pow(u, 2) + yDerivativeCoeff.b * u + yDerivativeCoeff.c,
            zDerivativeCoeff.a * pow(u, 2) + zDerivativeCoeff.b * u + zDerivativeCoeff.c
        );


        glm::vec3 normal = glm::normalize(glm::cross(up, tangent));
        normalSamples.emplace_back(InterpolationNode<glm::vec3>{ (float) u, normal });

        u += step;
    }
}

std::vector<float> SplineInterpolation::calculateChordalLength(const std::vector<glm::vec3> &nodes) {
    std::vector<float> lengths = {};
    for (int i = 1; i < nodes.size(); i++) {
        lengths.push_back(glm::length(nodes[i-1] - nodes[i]));
    }

    lengths.push_back(glm::length(*nodes.end() - nodes[0]));

    return lengths;
}

std::vector<InterpolationNode<glm::vec3>> SplineInterpolation::makeInterpolationNodes(const std::vector<glm::vec3> &nodes, std::vector<float> chords) const {
    std::vector<InterpolationNode<glm::vec3>> mapping = { { 0.0f, nodes[0] } };

    float currentLength = 0.0;
    for (int i = 1; i < nodes.size(); i++) {
        currentLength += chords[i-1];
        mapping.push_back({ currentLength, nodes[i] });
    }

    mapping.push_back({ totalLength, nodes[0]});

    return mapping;
}

std::vector<InterpolationNode<QubicCoeff>> SplineInterpolation::solveAxis(std::vector<InterpolationNode<glm::vec3>> interpolationNodes, int axis) {
    const int splineCount = interpolationNodes.size() - 1;
    const int size = splineCount * 4;
    Eigen::SparseMatrix<double> a(size, size);
    Eigen::VectorXd b(size), x(size);

    std::vector<Eigen::Triplet<double>> triplets;

    for (int i = 0; i < splineCount; i++) {
        int k = 4 * i;
        triplets.emplace_back( k, k, pow(interpolationNodes[i].t, 3) );
        triplets.emplace_back(k, k+1, pow(interpolationNodes[i].t, 2));
        triplets.emplace_back(k, k+2, interpolationNodes[i].t);
        triplets.emplace_back(k, k+3, 1);
        b[k] = interpolationNodes[i].value[axis];

        triplets.emplace_back(k+1, k, pow(interpolationNodes[i+1].t, 3));
        triplets.emplace_back(k+1, k+1, pow(interpolationNodes[i+1].t, 2));
        triplets.emplace_back(k+1, k+2, interpolationNodes[i+1].t);
        triplets.emplace_back(k+1, k+3, 1);
        b[k+1] = interpolationNodes[i+1].value[axis];

        if (i < splineCount-1) {
            triplets.emplace_back(k+2, k, 3 * pow(interpolationNodes[i+1].t, 2));
            triplets.emplace_back(k+2, k+1, 2 * interpolationNodes[i+1].t);
            triplets.emplace_back(k+2, k+2, 1);
            triplets.emplace_back(k+2, k+4, -3 * pow(interpolationNodes[i+1].t, 2));
            triplets.emplace_back(k+2, k+5, -2 * interpolationNodes[i+1].t);
            triplets.emplace_back(k+2, k+6, -1);
            b[k+2] = 0;

            triplets.emplace_back(k+3, k, 6 * interpolationNodes[i+1].t);
            triplets.emplace_back(k+3, k+1, 2);
            triplets.emplace_back(k+3, k+4, -6 * interpolationNodes[i+1].t);
            triplets.emplace_back(k+3, k+5, -2);
            b[k+3] = 0;
        }
    }

    triplets.emplace_back(size-2, 0, 3 * pow(interpolationNodes[0].t, 2));
    triplets.emplace_back(size-2, 1, 2 * interpolationNodes[0].t);
    triplets.emplace_back(size-2, 2, 1);
    triplets.emplace_back(size-2, size-4, -3 * pow(interpolationNodes[splineCount].t, 2));
    triplets.emplace_back(size-2, size-3, -2 * interpolationNodes[splineCount].t);
    triplets.emplace_back(size-2, size-2, -1);
    b[size-2] = 0;

    triplets.emplace_back(size-1, 0, 6 * interpolationNodes[0].t);
    triplets.emplace_back(size-1, 1, 2);
    triplets.emplace_back(size-1, size-4, -6 * interpolationNodes[splineCount].t);
    triplets.emplace_back(size-1, size-3, -2);
    b[size-1] = 0;

    a.setFromTriplets(triplets.begin(), triplets.end());

    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.analyzePattern(a);
    solver.factorize(a);
    if (solver.info() == Eigen::Success) {
        x = solver.solve(b);
    }

    std::vector<InterpolationNode<QubicCoeff>> results;
    for (int i = 0; i < splineCount; i++) {
        results.emplace_back(InterpolationNode<QubicCoeff>{
            interpolationNodes[i].t,
            QubicCoeff{ (float) x[4*i], (float) x[4*i+1], (float) x[4*i+2], (float) x[4*i+3] }
        });
    }

    return results;
}

int SplineInterpolation::bisectSamples(float t) const {
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

SplineInterpolation::SplineInterpolation() {
    samples = { InterpolationNode<glm::vec3>{ 0.0f, glm::vec3() } };
}

glm::vec3 SplineInterpolation::sample(float t, bool normal) {
    std::vector<InterpolationNode<glm::vec3>> space = normal ? normalSamples : samples;

    int leftmostIndex = bisectSamples(t);
    if (leftmostIndex+1 == space.size()) {
        return space[leftmostIndex].value;
    }

    float diffT = space[leftmostIndex+1].t - space[leftmostIndex].t;
    float lerpT = (space[leftmostIndex+1].t - t) / diffT;

    glm::vec3 v1 = space[leftmostIndex].value;
    glm::vec3 v2 = space[leftmostIndex+1].value;

    return {
        v1.x * lerpT + v2.x * (1-lerpT),
        v1.y * lerpT + v2.y * (1-lerpT),
        v1.z * lerpT + v2.z * (1-lerpT)
    };
}

glm::vec3 SplineInterpolation::samplePosition(float t) {
    return sample(t, false);
}

glm::vec3 SplineInterpolation::sampleNormal(float t) {
    return sample(t, true);
}
