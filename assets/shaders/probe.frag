#version 460 core

in vec3 normal;

uniform int skipNormal;
uniform vec3 overrideColor;

out vec4 color;

void main() {
    color = vec4(normal * (1 - skipNormal) + overrideColor, 1.0f);
}
