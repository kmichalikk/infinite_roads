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
FetchContent_MakeAvailable(glfw glm)

add_executable(racetrack_builder src/main.cpp
    src/game.cpp
    src/game.h
    src/macros.h
)

add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad SYSTEM PUBLIC vendor/glad/include)

target_link_libraries(
    racetrack_builder
    glad
    glfw
    glm
)