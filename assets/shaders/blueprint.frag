#version 460 core

#define MAX_POINT_LIGTHS 20

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    float ambient;
    vec3 diffuse;
};

in vec3 continuousPosition;
in vec2 texCoord;

uniform bool blueprint;
uniform sampler2D groundTexture;
uniform PointLight pointLights[MAX_POINT_LIGTHS];
uniform int pointLightsCount;

out vec4 color;

vec3 calculatePointLight(PointLight light);

void main() {
    vec2 coords = continuousPosition.xz;
    vec2 integerCoords = vec2(round(continuousPosition.x), round(continuousPosition.z));
    vec2 diffCoords = integerCoords - coords;
    int mask = abs(diffCoords.x) < 0.02 || abs(diffCoords.y) < 0.02 ? 1 : 0;

    if (blueprint) {
        color = vec4(0.07, 0.08, 0.11, 1.0) + mask * vec4(0.3, 0.4, 0.6, 1.0);
    } else {
        vec3 tmpColor = vec3(0, 0, 0);
        for (int i = 0; i < min(MAX_POINT_LIGTHS, pointLightsCount); i++) {
            tmpColor += calculatePointLight(pointLights[i]);
        }

        color = vec4(tmpColor, 1.0f);
    }
}

vec3 calculatePointLight(PointLight light) {
    vec3 lightDir = normalize(light.position - continuousPosition);
    float diff = max(dot(vec3(0, 1, 0), lightDir), 0.0);
    float distance = length(light.position - continuousPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 base = vec3(texture(groundTexture, texCoord));

    return (light.ambient + light.diffuse * diff) * base;
}
