#include "car.h"

#include <iostream>
#include <glad/glad.h>

#include "../resource_manager.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"

Car::Car()
    : Model("car", "car", "../assets/models/car.glb") {
}

void Car::calculateAngles() {
    glm::vec2 xUnit(1.0f, 0.0f);
    glm::vec2 flatNormal = -glm::normalize(glm::vec2(normal.x, -normal.z));
    normalAngle = glm::orientedAngle(xUnit, flatNormal);
    tangentAngle = normalAngle - M_PI / 2.0f;
    setRotation(glm::vec3(0, normalAngle, 0));
}

void Car::setNormal(glm::vec3 normal) {
    this->normal = normal;
    calculateAngles();
}
