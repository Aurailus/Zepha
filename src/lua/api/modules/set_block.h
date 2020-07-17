//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include "../../../def/DefinitionAtlas.h"
#include "../../../game/scene/world/World.h"

namespace Api {
    static void set_block(sol::table &core, DefinitionAtlas& defs, World& world) {
        core.set_function("set_block", [&](sol::table pos, std::string identifier) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) throw std::runtime_error("expected a vector as the first argument.");
            auto& block = defs.fromStr(identifier);
            world.setBlock({pos.get<float>("x"), pos.get<float>("y"), pos.get<float>("z")}, block.index);
        });
    }
}