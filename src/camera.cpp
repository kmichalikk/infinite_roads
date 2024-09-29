#include "camera.h"

#include <iostream>

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
    eventDispatcher.subscribe(MOUSESCROLL, [this](void *event){ handleScroll(event); });
}

void Camera::handleMouse(void *rawEvent) {
    MouseEvent *event = static_cast<MouseEvent *>(rawEvent);
    if (grab) {
        recalculate(event->position - prevMousePosition);
    }

    prevMousePosition = event->position;
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

void Camera::handleScroll(void *rawEvent) {
    MouseScrollEvent *event = static_cast<MouseScrollEvent *>(rawEvent);
    lastScrollDirection = event->direction;
    lastScrollEvent = glfwGetTime();
}

void Camera::recalculate(glm::vec2 mouseDiff) {
    origin.x -= mouseDiff.x * moveScale;
    origin.z -= mouseDiff.y * moveScale;
    position = origin + offsetVector;
    viewMatrix = glm::lookAt(position, origin, glm::vec3(0, 1.0, 0));
}

glm::vec3 Camera::getRayIntersection(const GameConfig &config) const {
    glm::vec4 rayClip(
        (2.0f * prevMousePosition.x) / config.screenWidth - 1.0f,
        1.0f - (2.0f * prevMousePosition.y) / config.screenHeight,
        -1.0f,
        1.0f
    );

    glm::vec4 rayEye = glm::inverse(config.projectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    glm::vec3 rayWorld = glm::inverse(viewMatrix) * rayEye;
    rayWorld = normalize(rayWorld);

    float t = position.y / rayWorld.y;

    return position - rayWorld * t;
}

void Camera::update(double dt) {
    if (lastScrollEvent + 0.1 > glfwGetTime()) {
        scrollSpeed = lastScrollDirection * 2;
    } else {
        scrollSpeed *= 0.5;
    }

    glm::vec3 newOffsetVector = offsetVector - offsetVector * scrollSpeed * (float) dt;

    if (newOffsetVector.y > 5 && newOffsetVector.y < 40) {
        offsetVector = newOffsetVector;
    }

    recalculate(glm::vec2());
}
