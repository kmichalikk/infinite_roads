#version 460 core

in vec3 continuousPosition;
in vec2 texCoord;

uniform bool blueprint;
uniform sampler2D groundTexture;

out vec4 color;

void main() {
    vec2 coords = continuousPosition.xz;
    vec2 integerCoords = vec2(round(continuousPosition.x), round(continuousPosition.z));
    vec2 diffCoords = integerCoords - coords;
    int mask = abs(diffCoords.x) < 0.02 || abs(diffCoords.y) < 0.02 ? 1 : 0;

    if (blueprint) {
        color = vec4(0.07, 0.08, 0.11, 1.0) + mask * vec4(0.3, 0.4, 0.6, 1.0);
    } else {
        color = texture(groundTexture, texCoord);
    }
}
