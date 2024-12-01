#pragma once

#include <string>
#include <vector>

#include "lights.h"
#include "glm/glm.hpp"

class Shader {
private:
    unsigned int shaderId = 0;
    static void checkShaderCompilationErrors(unsigned int shaderId);
    static void checkProgramLinkingErrors(unsigned int programId);
public:
    void use() const;
    void compile(const char *vertexSource, const char *fragmentSource);
    unsigned int getShaderId() const { return shaderId; };

    void setFloat(std::string name, float value) const;
    void setInt(std::string name, int value) const;
    void setVec3f(std::string name, float x, float y, float z) const;
    void setVec3f(std::string name, glm::vec3 other) const;
    void setMat4(std::string name, glm::mat4 other) const;
    void setPointLights(std::vector<PointLight> lights) const;
};
