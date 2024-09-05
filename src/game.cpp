#include "game.h"
#include "macros.h"
#include "resource_manager.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "render_nodes/blueprint.h"
#include "render_nodes/probe.h"

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

    config->projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float) config->screenWidth / (float) config->screenHeight,
        0.1f,
        100.0f
    );
}

bool Game::initializeLibraries() {
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

        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        ERROR("INIT", "Failed to initialize GLAD");
        glfwTerminate();

        return false;
    }

    glfwSetWindowUserPointer(window, &config);

    glViewport(0, 0, config.screenWidth, config.screenHeight);
    glfwSetFramebufferSizeCallback(window, resizeWindowCallback);

    return true;
}

void Game::registerRenderNodes() {
    renderNodes.push_back(std::make_unique<Blueprint>(100));
    renderNodes.push_back(std::make_unique<Probe>(glm::vec3(0.0, 0.0, 0.0)));
}

void Game::initialize() {
    if (!initializeLibraries()) {
        return;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    config.projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float) config.screenWidth / (float) config.screenHeight,
        0.1f,
        100.0f
    );

    config.viewMatrix = glm::lookAt(glm::vec3(2.0, 4.0, 5.0), glm::vec3(0), glm::vec3(0, 1, 0));

    ResourceManager::loadShader("probe");
    ResourceManager::loadShader("blueprint");

    registerRenderNodes();

    lastFrameTime = glfwGetTime();
    config.ready = true;
}

bool Game::ready() const {
    return config.ready;
}

void Game::startMainLoop() {
    if (!ready()) {
        ERROR("INIT", "Call initialize() first!");

        exit(1);
    }

    LOG("INIT", "Main loop started");
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double currentFrameTime = glfwGetTime();
        double deltaTime = lastFrameTime - currentFrameTime;
        lastFrameTime = currentFrameTime;

        for (const auto &node : renderNodes) {
            node->prepare();
            node->setShaderProjectionMatrix(config.projectionMatrix);
            node->setShaderViewMatrix(config.viewMatrix);
            node->draw(deltaTime);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
