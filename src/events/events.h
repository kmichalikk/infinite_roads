#pragma once

#include "glm/glm.hpp"

enum EventType {
    MOUSEMOVE,
    MOUSEBUTTON,
};

struct MouseEvent {
    glm::vec2 position;
};

struct MouseButtonEvent {
    glm::vec2 position;
    int button;
    int action;
};
