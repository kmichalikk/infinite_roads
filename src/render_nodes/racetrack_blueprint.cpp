#include "racetrack_blueprint.h"

#include <iostream>

void RacetrackBlueprint::addInterpolationNode(glm::vec3 position) {
    if (loopMade) {
        return;
    }

    Highlight highlight(glm::vec3(1.0f));
    highlight.setPosition(position);
    interpolationNodes.push_back(highlight);

    if (interpolationNodes.size() == 4) {
        loopPossible = true;
        interpolationNodes.at(0).setColor(glm::vec3(0.57, 0.9, 0.3));
    }
}

void RacetrackBlueprint::draw(double dt) {
    for (auto node : interpolationNodes) {
        node.prepare();
        node.draw(dt);
    }
}

void RacetrackBlueprint::setShaderViewMatrix(glm::mat4 view) const {
    for (auto node : interpolationNodes) {
        node.setShaderViewMatrix(view);
    }
}

void RacetrackBlueprint::setShaderProjectionMatrix(glm::mat4 projection) const {
    for (auto node : interpolationNodes) {
        node.setShaderProjectionMatrix(projection);
    }
}

bool RacetrackBlueprint::snapToFirst(Highlight *highlight) {
    if (!loopPossible || interpolationNodes.size() == 0) {
        return false;
    }

    glm::vec3 firstPosition = interpolationNodes.at(0).getPosition();
    if (glm::length(highlight->getPosition() - firstPosition) < 0.5) {
        highlight->setPosition(firstPosition);

        return true;
    }

    return false;
}

void RacetrackBlueprint::finish() {
    if (loopPossible) {
        loopMade = true;
        interpolationNodes.at(0).setColor(glm::vec3(1.0f));
    }
}
