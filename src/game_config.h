#pragma once

#include "glm/glm.hpp"

struct GameConfig {
    int screenWidth, screenHeight;
    int ready;
    glm::mat4 projectionMatrix;
};
