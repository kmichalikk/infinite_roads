#pragma once

#include "model.h"
#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL

class Car : public Model {
private:
    glm::vec3 normal;
    float tangentAngle;
    float normalAngle;

    void calculateAngles();
public:
    Car();
    void setNormal(glm::vec3 normal);
    void setPosition(glm::vec3 position) { Model::setPosition(position + 0.1f); };
};
