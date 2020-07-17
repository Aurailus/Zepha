//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include "../../../def/DefinitionAtlas.h"
#include "../../../game/scene/world/World.h"

namespace Api {
    static void get_block(sol::table &core, DefinitionAtlas& defs, World& world) {
        core.set_function("get_block", [&](sol::table pos) -> std::string {
            if (!pos["x"] || !pos["y"] || !pos["z"]) throw std::runtime_error("expected a vector as the first argument.");
            return defs.fromId(world.getBlock({pos.get<float>("x"), pos.get<float>("y"), pos.get<float>("z")})).identifier;
        });
    }
}
