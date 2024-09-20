cmake_minimum_required(VERSION 3.28)

include(FetchContent)

project(racetrack_builder)

set(CMAKE_CXX_STANDARD 17)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579
    SYSTEM
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
    SYSTEM
)

FetchContent_Declare(
    Eigen3 SYSTEM
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3147391d946bb4b6c68edd901f2add6ac1f31f8c
    SOURCE_SUBDIR nonexistent_do_not_configure_make_targets
)
FetchContent_MakeAvailable(glfw glm Eigen3)

# https://stackoverflow.com/questions/77210209
if(NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    set_target_properties(Eigen3::Eigen PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${eigen3_SOURCE_DIR})
endif()

add_executable(racetrack_builder src/main.cpp
    src/game.cpp
    src/game.h
    src/macros.h
    src/render_nodes/render_node.h
    src/render_nodes/render_node.cpp
    src/render_nodes/Probe.cpp
    src/render_nodes/Probe.h
    src/resource_manager.cpp
    src/resource_manager.h
    src/shader.cpp
    src/shader.h
    src/render_nodes/blueprint.cpp
    src/render_nodes/blueprint.h
    src/camera.cpp
    src/camera.h
    src/events/event_dispatcher.cpp
    src/events/event_dispatcher.h
    src/events/events.h
    src/game_config.h
    src/render_nodes/highlight.cpp
    src/render_nodes/highlight.h
    src/render_nodes/racetrack_blueprint.cpp
    src/render_nodes/racetrack_blueprint.h
    src/spline_interpolation.cpp
    src/spline_interpolation.h
    src/render_nodes/car.cpp
    src/render_nodes/car.h
    src/render_nodes/road.cpp
    src/render_nodes/road.h
)

add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad SYSTEM PUBLIC vendor/glad/include)

target_link_libraries(
    racetrack_builder
    glad
    glfw
    glm
    Eigen3::Eigen
)