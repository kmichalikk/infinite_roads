#version 460 core

uniform vec3 baseColor;

out vec4 color;

void main() {
    color = vec4(baseColor, 1.0);
}
