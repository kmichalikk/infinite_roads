#include "shader.h"

#include <glad/glad.h>

#include "macros.h"
#include "glm/gtc/type_ptr.hpp"

void Shader::compile(const char *vertexSource, const char *fragmentSource) {
    unsigned int vertexId, fragmentId;

    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vertexSource, nullptr);
    glCompileShader(vertexId);
    checkShaderCompilationErrors(vertexId);

    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentId);
    checkShaderCompilationErrors(fragmentId);

    shaderId = glCreateProgram();
    glAttachShader(shaderId, vertexId);
    glAttachShader(shaderId, fragmentId);
    glLinkProgram(shaderId);
    checkProgramLinkingErrors(shaderId);

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void Shader::use() const {
    glUseProgram(shaderId);
}

void Shader::checkShaderCompilationErrors(unsigned int shaderId) {
    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (success) {
        return;
    }

    char buf[1024];
    glGetShaderInfoLog(shaderId, 1024, nullptr, buf);

    ERROR("SHADER", "Shader compilation error: " + std::string(buf));
}

void Shader::checkProgramLinkingErrors(unsigned int programId) {
    int success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    if (success) {
        return;
    }

    char buf[1024];
    glGetProgramInfoLog(programId, 1024, nullptr, buf);

    ERROR("SHADER", "Linking error: " + std::string(buf));
}

void Shader::setFloat(std::string name, float value) const {
    glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::setInt(std::string name, int value) const {
    glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::setVec3f(std::string name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(shaderId, name.c_str()), x, y, z);
}

void Shader::setVec3f(std::string name, glm::vec3 other) const {
    glUniform3f(glGetUniformLocation(shaderId, name.c_str()), other.x, other.y, other.z);
}

void Shader::setMat4(std::string name, glm::mat4 other) const {
    auto loc = glGetUniformLocation(shaderId, name.c_str());
    glUniformMatrix4fv(loc, 1, false, glm::value_ptr(other));
}

void Shader::setPointLights(std::vector<PointLight> lights) const {
    int pointLightsCount = std::min(MAX_POINT_LIGHTS, (int) lights.size());

    setInt("pointLightsCount", pointLightsCount);

    std::string positionLocString("pointLights[0].position");
    std::string constantLocString("pointLights[0].constant");
    std::string linearLocString("pointLights[0].linear");
    std::string quadraticLocString("pointLights[0].quadratic");
    std::string ambientLocString("pointLights[0].ambient");
    std::string diffuseLocString("pointLights[0].diffuse");

    for (int i = 0; i < pointLightsCount; i++) {
        setVec3f(positionLocString, lights[i].position);
        positionLocString.replace(12, std::string::npos, std::to_string(i+1) + "].position");

        setFloat(constantLocString, lights[i].constant);
        constantLocString.replace(12, std::string::npos, std::to_string(i+1) + "].constant");

        setFloat(linearLocString, lights[i].linear);
        linearLocString.replace(12, std::string::npos, std::to_string(i+1) + "].linear");

        setFloat(quadraticLocString, lights[i].quadratic);
        quadraticLocString.replace(12, std::string::npos, std::to_string(i+1) + "].quadratic");

        setFloat(ambientLocString, lights[i].ambient);
        ambientLocString.replace(12, std::string::npos, std::to_string(i+1) + "].ambient");

        setVec3f(diffuseLocString, lights[i].diffuse);
        diffuseLocString.replace(12, std::string::npos, std::to_string(i+1) + "].diffuse");
    }
}
