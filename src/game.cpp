#include <iomanip>
#include "game.h"
#include "macros.h"
#include "resource_manager.h"
#include "spline_interpolation.h"
#include "events/event_dispatcher.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "render_nodes/debug_vectors.h"
#include "render_nodes/ground.h"
#include "render_nodes/model.h"
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

void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    MouseScrollEvent *event = new MouseScrollEvent;
    event->direction = (int) (yOffset / abs(yOffset));
    eventDispatcher.dispatch(MOUSESCROLL, event);
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
    glfwSetScrollCallback(window, mouseScrollCallback);

    return true;
}

void Game::registerRenderNodes() {
    ground = std::make_shared<Ground>(100);
    renderNodes.push_back(ground);
    renderNodes.push_back(std::make_shared<Probe>(glm::vec3(0.0, 0.5, 0.0)));

    highlight = std::make_shared<Highlight>(glm::vec3(0.5f, 0.6f, 0.8f), "highlightCursor");

    renderNodes.push_back(highlight);

    racetrackBlueprint = std::make_shared<RacetrackBlueprint>();
    renderNodes.push_back(racetrackBlueprint);
    eventDispatcher.subscribe(EventType::MOUSEBUTTON, [this](void * event){ raycastClick(event); });

    car = std::make_shared<Car>(); // only create, will be added after racetrack is finished
    road = std::make_shared<Road>(); // only create, will be added after racetrack is finished
}

void Game::raycastClick(void *mouseButtonEvent) {
    MouseButtonEvent *event = static_cast<MouseButtonEvent *>(mouseButtonEvent);
    if (event->button == GLFW_MOUSE_BUTTON_LEFT && event->action == GLFW_PRESS) {
        if (racetrackBlueprint->snapToFirst(highlight.get())) {
            interpolation = racetrackBlueprint->finish();
            simulation.initialize(
                car,
                std::make_shared<Sampler<glm::vec3>>(interpolation.getPositionSampler()),
                std::make_shared<Sampler<glm::vec3>>(interpolation.getUnitNormalSampler()),
                std::make_shared<Sampler<glm::vec3>>(interpolation.getTangentSampler()),
                std::make_shared<Sampler<glm::vec3>>(interpolation.getAccelerationSampler())
            );
            simulation.update(0.0);

            road->sampleFrom(interpolation);
            renderNodes.insert(renderNodes.begin(), road);
            renderNodes.insert(renderNodes.begin(), car);
            ground->toggleBlueprint();
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
    ResourceManager::loadShader("color");
    ResourceManager::loadShader("road");
    ResourceManager::loadShader("car");

    ResourceManager::loadTexture("asphalt");
    ResourceManager::loadTexture("grass");

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
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        highlight->setPosition(camera.getRayIntersection(config));
        racetrackBlueprint->snapToFirst(highlight.get());

        if (racetrackBlueprint->finished()) {
            simulation.update(deltaTime);
        }

        camera.update(deltaTime);
        glm::mat4 viewMatrix = camera.getViewMatrix();

        for (const auto &node : renderNodes) {
            node->draw(deltaTime, &config.projectionMatrix, &viewMatrix);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
