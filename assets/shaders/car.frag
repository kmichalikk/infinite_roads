#version 460 core

in vec3 normal;
in vec2 textureCoords;

uniform sampler2D texture_diffuse0;

out vec4 color;

void main() {
     color = texture(texture_diffuse0, textureCoords);
}
