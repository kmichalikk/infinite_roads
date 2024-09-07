#version 460 core

in float alpha;

out vec4 color;

void main() {
    color = vec4(0.5, 0.6, 0.8, alpha);
}
