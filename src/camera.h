#pragma once

#include "game_config.h"
#include "events/events.h"
#include "glm/glm.hpp"


class Camera {
private:
    glm::vec3 position;
    glm::vec3 origin;
    glm::vec3 offsetVector;
    glm::mat4 viewMatrix;

    const float moveScale = 0.01;

    bool grab = false;
    glm::vec2 prevMousePosition;
    void handleMouse(void *);
    void handleMouseButton(void *);

    void recalculate(glm::vec2 mouseDiff);
public:
    Camera();
    glm::mat4 getViewMatrix() const { return viewMatrix; };
    glm::vec3 getRayIntersection(const GameConfig &config) const;
};
