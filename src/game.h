#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"

struct GameConfig {
    int screenWidth, screenHeight;
    int ready;
};

class Game {
private:
    GameConfig config;
    GLFWwindow* window;
    static void resizeWindowCallback(GLFWwindow *window, int width, int height);
public:
    Game(int screenWidth, int screenHeight);
    void initialize();
    bool ready() const;
    void startMainLoop();
};
