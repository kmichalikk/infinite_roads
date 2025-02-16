cmake_minimum_required(VERSION 3.28)

include(FetchContent)

project(racetrack_builder)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

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
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glm)

add_subdirectory(vendor/assimp-5.4.3)

# https://stackoverflow.com/questions/77210209
if(NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    set_target_properties(Eigen3::Eigen PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/vendor/eigen-3.4.0)
endif()

add_executable(racetrack_builder
    src/main.cpp
    src/game.cpp
    src/render_nodes/render_node.cpp
    src/render_nodes/probe.cpp
    src/resource_manager.cpp
    src/resource_manager.h
    src/shader.cpp
    src/render_nodes/ground.cpp
    src/camera.cpp
    src/events/event_dispatcher.cpp
    src/render_nodes/highlight.cpp
    src/render_nodes/racetrack_blueprint.cpp
    src/spline_interpolation.cpp
    src/render_nodes/car.cpp
    src/render_nodes/road.cpp
    src/texture.cpp
    src/render_nodes/mesh.cpp
    src/render_nodes/model.cpp
    src/render_nodes/debug_vectors.cpp
    src/simulation.cpp
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

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
