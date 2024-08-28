#include "resource_manager.h"

#include <fstream>
#include <sstream>

#include "macros.h"

const std::string SHADERS_DIR = "../assets/shaders/";

std::map<std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::loadShader(std::string name, std::string vertexFileName, std::string fragmentFileName) {
    std::string vertexSource = readShaderFile(SHADERS_DIR + vertexFileName);
    std::string fragmentSource = readShaderFile(SHADERS_DIR + fragmentFileName);

    Shader shader;
    shader.compile(vertexSource.c_str(), fragmentSource.c_str());

    shaders[name] = shader;

    return shader;
}

Shader ResourceManager::loadShader(std::string name) {
    return loadShader(name, name + ".vert", name + ".frag");
}

Shader ResourceManager::getShader(std::string name) {
    if (shaders.find(name) == shaders.end()) {
        ERROR("RESOURCE", "Shader \"" + name + "\" not found");
    }

    return shaders[name];
}

std::string ResourceManager::readShaderFile(std::string fileName) {
    std::ifstream fin(fileName);
    std::string line;
    std::stringstream readStream;
    if (fin.is_open()) {
        while (std::getline(fin, line)) {
            readStream << line << '\n';
        }
    } else {
        ERROR("IO", "Failed to read file \"" + fileName + "\"");
    }

    fin.close();

    return readStream.str();
}
