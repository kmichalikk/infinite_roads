#include "racetrack_blueprint.h"

#include <algorithm>
#include <iostream>

#include "../spline_interpolation.h"

void RacetrackBlueprint::addInterpolationNode(glm::vec3 position) {
    if (loopMade) {
        return;
    }

    std::shared_ptr<Highlight> highlight = std::make_shared<Highlight>(
        glm::vec3(1.0f),
        "highlight" + std::to_string(getChildCount())
    );
    highlight->setPosition(position);
    addChild(highlight);
    positions.push_back(position);

    if (getChildCount() == 4) {
        loopPossible = true;
        std::static_pointer_cast<Highlight>(getChild("highlight0"))->setColor(glm::vec3(0.57, 0.9, 0.3));
    }
}

bool RacetrackBlueprint::snapToFirst(Highlight *highlight) {
    if (!loopPossible || getChildCount() == 0) {
        return false;
    }

    glm::vec3 firstPosition = getChild("highlight0")->getPosition();
    if (glm::length(highlight->getPosition() - firstPosition) < 0.5) {
        highlight->setPosition(firstPosition);

        return true;
    }

    return false;
}

SplineInterpolation RacetrackBlueprint::finish() {
    if (!loopPossible) {
        throw "invalid state";
    }

    std::static_pointer_cast<Highlight>(getChild("highlight0"))->setColor(glm::vec3(1.0f));
    SplineInterpolation interpolation(positions);
    loopMade = true;

    return interpolation;
}
