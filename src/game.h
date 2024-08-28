#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "render_nodes/render_node.h"

struct GameConfig {
    int screenWidth, screenHeight;
    int ready;
};

class Game {
private:
    GameConfig config;
    GLFWwindow* window;
    double lastFrameTime;
    std::vector<std::unique_ptr<RenderNode>> renderNodes;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    static void resizeWindowCallback(GLFWwindow *window, int width, int height);
    bool initializeLibraries();
    void registerRenderNodes();
public:
    Game(int screenWidth, int screenHeight);
    void initialize();
    bool ready() const;
    void startMainLoop();
};
