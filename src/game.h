#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "camera.h"
#include "game_config.h"
#include "render_nodes/ground.h"
#include "render_nodes/car.h"
#include "render_nodes/highlight.h"
#include "render_nodes/racetrack_blueprint.h"
#include "render_nodes/render_node.h"
#include "render_nodes/road.h"

class Game {
private:
    GameConfig config;
    GLFWwindow* window;
    Camera camera;
    double lastFrameTime;
    double trackPosition = 0.0f;
    std::shared_ptr<Highlight> highlight;
    std::shared_ptr<RacetrackBlueprint> racetrackBlueprint;
    std::shared_ptr<Car> car;
    std::shared_ptr<Road> road;
    std::shared_ptr<Ground> ground;
    SplineInterpolation interpolation;
    std::vector<std::shared_ptr<RenderNode>> renderNodes;

    static void resizeWindowCallback(GLFWwindow *window, int width, int height);
    bool initializeLibraries();
    void registerRenderNodes();
    void raycastClick(void *mouseButtonEvent);
public:
    Game(int screenWidth, int screenHeight);
    void initialize();
    bool ready() const;
    void startMainLoop();
};
