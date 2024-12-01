#pragma once

#include <vector>

#include "lights.h"
#include "glm/glm.hpp"

struct GameConfig {
    int screenWidth, screenHeight;
    int ready;
    glm::mat4 projectionMatrix;
    std::vector<PointLight> pointLights;
};
