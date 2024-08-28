#pragma once
#include <map>
#include <string>

#include "shader.h"

class ResourceManager {
private:
    static std::map<std::string, Shader> shaders;
    static std::string readShaderFile(std::string fileName);
public:
    static Shader loadShader(std::string name, std::string vertexFileName, std::string fragmentFileName);
    static Shader loadShader(std::string name);
    static Shader getShader(std::string name);
};
