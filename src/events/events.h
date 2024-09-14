#pragma once

#include "glm/glm.hpp"

enum EventType {
    MOUSEMOVE,
    MOUSEBUTTON,
    MOUSESCROLL,
};

struct MouseEvent {
    glm::vec2 position;
};

struct MouseScrollEvent {
    int direction;
};

struct MouseButtonEvent {
    glm::vec2 position;
    int button;
    int action;
};
