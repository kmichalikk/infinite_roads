#version 460 core

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec2 lTexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 continuousPosition;
out vec2 texCoord;

void main() {
    gl_Position = projection * view * model * vec4(lPosition, 1);
    continuousPosition = lPosition;
    texCoord = lTexCoord;
}
