//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "../basic/GuiContainer.h"

#include "../basic/GuiRect.h"
#include "../../SerialGui.h"
#include "../../../../def/ClientGame.h"
#include "../../../scene/world/Inventory.h"
#include "../../../scene/world/InventoryList.h"

class GuiInventoryList : public GuiContainer {
public:
    GuiInventoryList() = default;
    GuiInventoryList(const std::string& key);
    ~GuiInventoryList() override;

    static std::shared_ptr<GuiInventoryList> fromSerialized(SerialGui::Elem s, ClientGame &game,
        glm::ivec2 bounds, Inventory& inventory, InventoryList& hand);

    void create(glm::vec2 scale, glm::vec4 padding, glm::ivec2 innerPadding, InventoryList& list, InventoryList& hand, ClientGame& defs);

    void setHoverCallback(const callback& hoverCallback) override;
    void setLeftClickCallback(const callback& leftClickCallback) override;
    void setRightClickCallback(const callback& rightClickCallback) override;

    void hoverEvent(bool hovered, glm::ivec2 pos);
    void leftClick(bool down, glm::ivec2 pos);
    void rightClick(bool down, glm::ivec2 pos);
    void drawContents();
private:
    std::shared_ptr<GuiRect> hoverRect = std::make_shared<GuiRect>("hover_rect");

    InventoryList* list;
    InventoryList* hand;
    ClientGame* defs;
    glm::ivec2 innerPadding;
};