//
// Created by aurailus on 27/07/19.
//

#include <utility>
#include "GUIComponent.h"

GUIComponent::GUIComponent(const std::string& key) :
    key(key) {}

void GUIComponent::setScale(glm::vec2 scale) {
    this->scale = scale;
    entity.setScale(scale);
}

glm::vec2 GUIComponent::getScale() {
    return scale;
}

void GUIComponent::setPadding(glm::vec4 padding) {
    this->padding = padding;
}

glm::vec4 GUIComponent::getPadding() {
    return padding;
}

void GUIComponent::setPos(glm::ivec2 pos) {
    this->pos = pos;
    if (parent != nullptr) {
        glm::vec2 parentPos = parent->entity.getPos();
        pos += glm::vec2 {parentPos.x, parentPos.y};
        pos += glm::vec2 {parent->getPadding().w, parent->getPadding().x};
    }
    entity.setPos(pos);
    for (const auto& child : children) {
        child->updatePos();
    }
}

glm::ivec2 GUIComponent::getPos() {
    return pos;
}

void GUIComponent::add(std::shared_ptr<GUIComponent> component) {
    component->parent = this;
    component->updatePos();
    children.push_back(std::move(component));
}

void GUIComponent::insert(unsigned int index, std::shared_ptr<GUIComponent> component) {
    component->parent = this;
    component->updatePos();
    children.insert(std::next(children.begin(), index), std::move(component));
}

void GUIComponent::remove(const std::string& key) {
    for (auto it = children.cbegin(); it != children.cend(); it++) {
        if (it->get()->key == key) {
            children.erase(it);
            return;
        }
    }
}

void GUIComponent::empty() {
    for (auto it = children.cbegin(); it != children.cend();) {
        it = children.erase(it);
    }
}

void GUIComponent::draw(Renderer& renderer) {
    entity.draw(renderer);
    for (const auto& child : children) {
        child->draw(renderer);
    }
}

void GUIComponent::setVisible(bool visible) {
    Drawable::setVisible(visible);
    entity.setVisible(visible);
    for (const auto& child : children) {
        child->setVisible(visible);
    }
}

void GUIComponent::updatePos() {
    glm::vec2 realPos(pos);
    if (parent != nullptr) {
        glm::vec2 parentPos = parent->entity.getPos();
        realPos += glm::vec2 {parentPos.x, parentPos.y};
        realPos += glm::vec2 {parent->getPadding().w, parent->getPadding().x};
    }
    entity.setPos(realPos);
    for (const auto& child : children) {
        child->updatePos();
    }
}

bool GUIComponent::mouseActivity(glm::ivec2 pos) {
    for (auto child = children.rbegin(); child != children.rend(); ++child) {
        if ((*child)->mouseActivity(pos - (*child)->getPos())) return true;
    }
    if (pos.x >= 0 && pos.y >= 0 && pos.x <= scale.x && pos.y <= scale.y) {
//        std::cout << key << std::endl; //TODO: Hover states
        return true;
    }
    return false;
}

bool GUIComponent::triggerClick(glm::ivec2 pos) {
    for (auto child = children.rbegin(); child != children.rend(); ++child) {
        if ((*child)->triggerClick(pos - (*child)->getPos())) return true;
    }
    if (pos.x >= 0 && pos.y >= 0 && pos.x <= scale.x && pos.y <= scale.y && cb != nullptr) {
        cb();
        return true;
    }
    return false;
}

void GUIComponent::setClickCallback(std::function<void()> cb) {
    this->cb = cb;
}

const std::string &GUIComponent::getKey() {
    return key;
}