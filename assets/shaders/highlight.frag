#version 460 core

in float alpha;

uniform vec3 highlightColor;

out vec4 color;

void main() {
    color = vec4(highlightColor, alpha);
}
