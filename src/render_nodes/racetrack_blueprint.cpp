#include "racetrack_blueprint.h"

void RacetrackBlueprint::addInterpolationNode(glm::vec3 position) {
    Highlight highlight(glm::vec3(1.0f));
    highlight.setPosition(position);
    interpolationNodes.push_back(highlight);
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
