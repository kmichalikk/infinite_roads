#pragma once

#include "glm/vec3.hpp"

#define MAX_POINT_LIGHTS 20

struct PointLight {
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    float ambient;
    glm::vec3 diffuse;

    PointLight(glm::vec3 position, glm::vec3 diffuse, float ambient)
        : position(position), constant(1.0f), linear(0.7f), quadratic(1.8f), ambient(ambient), diffuse(diffuse) {
    }
};
