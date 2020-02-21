//
// Created by aurailus on 2019-12-17.
//

#include "GUIModel.h"

#include "../../../../def/ClientGame.h"

GUIModel::GUIModel(const std::string &key) : GUIComponent(key) {}

std::shared_ptr<GUIModel> GUIModel::fromSerialized(SerialGui::Elem s, ClientGame &game, glm::ivec2 bounds) {
    glm::vec2 pos        = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position", bounds);
    glm::vec2 scale      = SerialGui::deserializeToken<glm::vec2>(s.tokens, "scale");
    glm::vec2 anim_range = SerialGui::deserializeToken<glm::vec2>(s.tokens, "anim_range");
    if (scale == glm::vec2{0, 0}) scale = {1, 1};

    std::string type    = s.tokens.count("type") ? s.tokens["type"] : "model";
    std::string source  = s.tokens["source"];
    std::string texture = s.tokens["texture"];

    auto m = std::make_shared<Model>();
    if (type == "model") m->fromSerialized(game.models.models[source], {game.textures[texture]});

    auto model = std::make_shared<GUIModel>(s.key);
    model->create(scale, m);
    model->setPos(pos);

    if (anim_range.y != 0) model->animate(anim_range);

    return model;
}

void GUIModel::create(glm::vec2 scale, std::shared_ptr<Model> model) {
    entity.setModel(model);
    setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});

    setRotationX(180);
    setRotationY(215);
}

void GUIModel::update(double delta) {
    entity.update(delta);
}

void GUIModel::animate(glm::vec2 range) {
    entity.playRange(range.x, range.y, true);
}

void GUIModel::setRotationX(float x) {
    entity.setRotateX(x);
}

void GUIModel::setRotationY(float y) {
    entity.setRotateY(y);
}

void GUIModel::setRotationZ(float z) {
    entity.setRotateZ(z);
}

void GUIModel::draw(Renderer &renderer) {
    renderer.toggleDepthTest(true);
    renderer.clearDepthBuffer();
    GUIComponent::draw(renderer);
    renderer.toggleDepthTest(false);
}
