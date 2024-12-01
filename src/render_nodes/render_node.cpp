#include "render_node.h"

#include "../macros.h"
#include "../resource_manager.h"
#include "glm/ext/matrix_transform.hpp"

RenderNode::RenderNode(std::string name)
    : RenderNode(name, name) {
}

RenderNode::RenderNode(std::string name, std::string shaderName)
    : parent(nullptr), position(0.0f), rotation(0.0f) {
    this->name = name;
    shader = ResourceManager::getShader(shaderName);
}

void RenderNode::draw(double dt, glm::mat4 *projection, glm::mat4 *view, GameConfig *config) {
    if (children.size() > 0) {
        for (std::pair<std::string, std::shared_ptr<RenderNode>> ch : children) {
            ch.second->shader.use();
            ch.second->shader.setMat4("projection", *projection);
            ch.second->shader.setMat4("view", *view);
            if (ch.second->supportsLights()) {
                ch.second->shader.setPointLights(config->pointLights);
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            ch.second->doDraw(dt, &model);
        }
    } else {
        shader.use();
        shader.setMat4("projection", *projection);
        shader.setMat4("view", *view);
        if (supportsLights()) {
            shader.setPointLights(config->pointLights);
        }

        glm::mat4 model = glm::mat4(1.0f);
        doDraw(dt, &model);
    }
}

/**
 * Default implementation does nothing - can be used for nodes
 * that draw only their children and don't have any geometry by themselves
 */
void RenderNode::doDraw(double dt, glm::mat4 *parentTransform) {
}

std::shared_ptr<RenderNode> RenderNode::getChild(std::string name) {
    if (children.count(name) == 0) {
        ERROR("SCENE", "No child with name " + name + " in " + getName());
        return nullptr;
    }

    return children[name];
}

void RenderNode::addChild(std::shared_ptr<RenderNode> child) {
    if (children.count(child->getName()) > 0) {
        ERROR("SCENE", "Adding two children with identical names");
        return;
    }

    child->setParent(this);
    children[child->getName()] = child;
}

void RenderNode::removeChild(std::string name) {
    if (children.count(name) == 0) {
        return;
    }

    children[name]->setParent(nullptr);
    children.erase(name);
}

void RenderNode::iterateChildren(std::function<void(std::shared_ptr<RenderNode>)> func) {
    for (std::pair<std::string, std::shared_ptr<RenderNode>> node : children) {
        func(node.second);
    }
}
