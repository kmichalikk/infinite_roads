#include "resource_manager.h"

#include <fstream>
#include <sstream>
#include <stb_image.h>

#include "macros.h"
#include "GLFW/glfw3.h"

const std::string SHADERS_DIR = "../assets/shaders/";
const std::string TEXTURES_DIR = "../assets/textures/";

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;

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
        glfwTerminate();
    }

    fin.close();

    return readStream.str();
}

Texture ResourceManager::loadTexture(std::string name) {
    std::string path = TEXTURES_DIR + "/" + name + "/color.jpg";
    int width, height, channelCount;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channelCount, 0);
    if (data == nullptr) {
        ERROR("IO", "Failed to load texture from \"" + path + "\"");
        glfwTerminate();
    }

    textures[name] = Texture{width, height, data};
    stbi_image_free(data);

    return textures[name];
}

Texture ResourceManager::getTexture(std::string name) {
    if (textures.find(name) == textures.end()) {
        ERROR("RESOURCE", "Texture \"" + name + "\" not found");
    }

    return textures[name];
}
