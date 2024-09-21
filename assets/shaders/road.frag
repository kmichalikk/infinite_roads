#version 460 core

in vec3 continuousPosition;
in vec2 texCoord;

uniform sampler2D roadTexture;

out vec4 color;

void main() {
    color = texture(roadTexture, texCoord);
//    color = vec4(fract(texCoord.x), fract(texCoord.y), 0.0f, 1.0f);
}
