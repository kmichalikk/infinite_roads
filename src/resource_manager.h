#pragma once
#include <map>
#include <string>

#include "shader.h"
#include "texture.h"

class ResourceManager {
private:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture> textures;
    static std::string readShaderFile(std::string fileName);
public:
    static Shader loadShader(std::string name, std::string vertexFileName, std::string fragmentFileName);
    static Shader loadShader(std::string name);
    static Shader getShader(std::string name);
    static Texture loadTexture(std::string name);
    static Texture getTexture(std::string name);
};
