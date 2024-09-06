#include "camera.h"

#include <iostream>

#include "macros.h"
#include "events/event_dispatcher.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera() {
    offsetVector = glm::vec3(0.0, 8.0, 5.0);
    origin = glm::vec3(0.0);
    position = origin + offsetVector;

    viewMatrix = glm::lookAt(position, origin, glm::vec3(0, 1.0, 0));

    eventDispatcher.subscribe(MOUSEMOVE, [this](void *event){ handleMouse(event); });
    eventDispatcher.subscribe(MOUSEBUTTON, [this](void *event){ handleMouseButton(event); });
}

void Camera::handleMouse(void *rawEvent) {
    MouseEvent *event = static_cast<MouseEvent *>(rawEvent);
    if (grab) {
        recalculate(event->position - prevMousePosition);
        prevMousePosition = event->position;
    }
}

void Camera::handleMouseButton(void *rawEvent) {
    MouseButtonEvent *event = static_cast<MouseButtonEvent *>(rawEvent);
    if (event->button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (event->action == GLFW_PRESS) {
            grab = true;
            prevMousePosition = event->position;
        } else {
            grab = false;
        }
    }
}

void Camera::recalculate(glm::vec2 mouseDiff) {
    origin.x -= mouseDiff.x * moveScale;
    origin.z -= mouseDiff.y * moveScale;
    position = origin + offsetVector;
    viewMatrix = glm::lookAt(position, origin, glm::vec3(0, 1.0, 0));
}
