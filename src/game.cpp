#include "game.h"
#include "macros.h"
#include "resource_manager.h"
#include "events/event_dispatcher.h"
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

void mouseCallback(GLFWwindow *window, double x, double y) {
    MouseEvent *event = new MouseEvent;
    event->position = glm::vec2(x, y);
    eventDispatcher.dispatch(EventType::MOUSEMOVE, event);
    delete event;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    MouseButtonEvent *event = new MouseButtonEvent;
    event->action = action;
    event->button = button;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    event->position = glm::vec2(x, y);
    eventDispatcher.dispatch(EventType::MOUSEBUTTON, event);
    delete event;
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
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    return true;
}

void Game::registerRenderNodes() {
    renderNodes.push_back(std::make_shared<Blueprint>(100));
    renderNodes.push_back(std::make_shared<Probe>(glm::vec3(0.0, 0.5, 0.0)));

    highlight = std::make_shared<Highlight>(glm::vec3(0.5f, 0.6f, 0.8f));
    renderNodes.push_back(highlight);

    racetrackBlueprint = std::make_shared<RacetrackBlueprint>();
    renderNodes.push_back(racetrackBlueprint);
    eventDispatcher.subscribe(EventType::MOUSEBUTTON, [this](void * event){ raycastClick(event); });
}

void Game::raycastClick(void *mouseButtonEvent) {
    MouseButtonEvent *event = static_cast<MouseButtonEvent *>(mouseButtonEvent);
    if (event->button == GLFW_MOUSE_BUTTON_LEFT && event->action == GLFW_PRESS) {
        if (racetrackBlueprint->snapToFirst(highlight.get())) {
            racetrackBlueprint->finish();
        } else {
            racetrackBlueprint->addInterpolationNode(camera.getRayIntersection(config));
        }
    }
}

void Game::initialize() {
    if (!initializeLibraries()) {
        return;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    config.projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float) config.screenWidth / (float) config.screenHeight,
        0.1f,
        100.0f
    );

    ResourceManager::loadShader("probe");
    ResourceManager::loadShader("blueprint");
    ResourceManager::loadShader("highlight");

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

        highlight->setPosition(camera.getRayIntersection(config));
        racetrackBlueprint->snapToFirst(highlight.get());

        for (const auto &node : renderNodes) {
            node->prepare();
            node->setShaderProjectionMatrix(config.projectionMatrix);
            node->setShaderViewMatrix(camera.getViewMatrix());
            node->draw(deltaTime);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
