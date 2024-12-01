#pragma once

#include <functional>
#include <map>
#include <memory>

#include "glm/glm.hpp"
#include "../shader.h"
#include "../game_config.h"

class RenderNode {
private:
    std::string name;
    RenderNode *parent;
    glm::vec3 position;
    glm::vec3 rotation;
protected:
    Shader shader;
    std::map<std::string, std::shared_ptr<RenderNode>> children;
public:
    RenderNode(std::string name);
    RenderNode(std::string name, std::string shaderName);

    virtual ~RenderNode() = default;

    void draw(double dt, glm::mat4 *projection, glm::mat4 *view, GameConfig *config);
    virtual void doDraw(double dt, glm::mat4 *parentTransform);

    std::shared_ptr<RenderNode> getChild(std::string name);
    void addChild(std::shared_ptr<RenderNode> child);
    void removeChild(std::string name);
    int getChildCount() const { return children.size(); }
    void iterateChildren(std::function<void(std::shared_ptr<RenderNode>)> func);

    std::string getName() const { return name; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getRotation() const { return rotation; }
    void setPosition(glm::vec3 position) { this->position = position; }
    void setRotation(glm::vec3 rotation) { this->rotation = rotation; }
    void setParent(RenderNode *parent) { this->parent = parent;  }
    virtual bool supportsLights() { return false; }
};
