#include "game.h"
#include "macros.h"

Game::Game(int screenWidth, int screenHeight) {
    config.screenWidth = screenWidth;
    config.screenHeight = screenHeight;
    config.ready = false;
}

void Game::resizeWindowCallback(GLFWwindow *window, int width, int height) {
    auto *config = static_cast<GameConfig*>(glfwGetWindowUserPointer(window));
    config->screenWidth = width;
    config->screenHeight = height;
    glViewport(0, 0, width, height);
}

void Game::initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        config.screenWidth,
        config.screenHeight,
        "RacetrackBuilder | New project",
        nullptr,
        nullptr
    );

    if (window == nullptr) {
        ERROR("INIT", "Failed to create GLFW window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        ERROR("INIT", "Failed to initialize GLAD");
        glfwTerminate();
    }

    glfwSetWindowUserPointer(window, &config);

    glViewport(0, 0, config.screenWidth, config.screenHeight);
    glfwSetFramebufferSizeCallback(window, resizeWindowCallback);
    config.ready = true;
}

bool Game::ready() const {
    return config.ready;
}

void Game::startMainLoop() {
    LOG("INIT", "Main loop started");
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
