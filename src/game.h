#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "camera.h"
#include "game_config.h"
#include "render_nodes/highlight.h"
#include "render_nodes/render_node.h"

class Game {
private:
    GameConfig config;
    GLFWwindow* window;
    Camera camera;
    double lastFrameTime;
    std::shared_ptr<Highlight> highlight;
    std::vector<std::shared_ptr<RenderNode>> renderNodes;

    static void resizeWindowCallback(GLFWwindow *window, int width, int height);
    bool initializeLibraries();
    void registerRenderNodes();
public:
    Game(int screenWidth, int screenHeight);
    void initialize();
    bool ready() const;
    void startMainLoop();
};
