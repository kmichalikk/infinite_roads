cmake_minimum_required(VERSION 3.28)

include(FetchContent)

project(racetrack_builder)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wno-unknown-pragmas")

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
# https://stackoverflow.com/questions/77210209
FetchContent_Declare(
    Eigen3 SYSTEM
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3147391d946bb4b6c68edd901f2add6ac1f31f8c
    SOURCE_SUBDIR nonexistent_do_not_configure_make_targets
)
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG c35200e38ea8f058812b83de2ef32c6093b0ece2
    SYSTEM
)
FetchContent_MakeAvailable(glfw glm Eigen3 assimp)

# https://stackoverflow.com/questions/77210209
if(NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen INTERFACE IMPORTED
        vendor/stb_image/impl.cpp
        vendor/stb_image/impl.cpp)
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
    src/render_nodes/Ground.cpp
    src/render_nodes/Ground.h
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
    src/texture.cpp
    src/texture.h
    src/render_nodes/mesh.cpp
    src/render_nodes/mesh.h
    src/render_nodes/model.cpp
    src/render_nodes/model.h
    src/sampler.cpp
    src/sampler.h
    src/interpolation_node.h
)

add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad SYSTEM PUBLIC vendor/glad/include)

add_library(stb_image vendor/stb_image/impl.cpp)
target_include_directories(stb_image SYSTEM PUBLIC vendor/stb_image)

target_link_libraries(
    racetrack_builder
    glad
    glfw
    glm
    Eigen3::Eigen
    stb_image
    assimp
)