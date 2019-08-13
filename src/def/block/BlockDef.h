//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_SERVERBLOCKDEF_H
#define ZEUS_SERVERBLOCKDEF_H

#include <string>
#include <sol2/sol.hpp>
#include "SelectionBox.h"
#include "BlockModel.h"
#include "../ItemDef.h"
#include "../../api/Callback.h"

class BlockDef : public ItemDef {
public:
    BlockDef() = default;
    BlockDef(const std::string& identifier, const BlockModel& model, bool solid, SelectionBox selectionBox);
    BlockDef(const std::string& identifier, unsigned int index, const BlockModel& model, bool solid, SelectionBox selectionBox);

    BlockModel model;
    bool culls = false;
    bool solid = false;

    SelectionBox selectionBox;

    std::unordered_map<Callback, sol::function, Util::EnumClassHash> callbacks {};
};

#endif //ZEUS_SERVERBLOCKDEF_H
