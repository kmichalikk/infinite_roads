#version 460 core

layout (location=0) in vec3 lPosition;
layout (location=1) in vec3 lNormal;
layout (location=2) in vec2 lTextureCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normal;
out vec2 textureCoords;

void main() {
    gl_Position = projection * view * model * vec4(lPosition, 1.0f);
    normal = lNormal;
    textureCoords = lTextureCoords;
}
