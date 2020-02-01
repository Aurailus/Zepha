//
// Created by aurailus on 11/10/19.
//

#pragma once

#include <sol2/sol.hpp>
#include "../type/LocalLuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"

namespace Api {
    //TODO: Clean method
    static void remove_entity_c(sol::state& lua, sol::table& core, LocalDefs& defs, LocalWorld& world) {
        core.set_function("remove_entity", [&](sol::table entity) {

            std::shared_ptr<LocalLuaEntity> object = entity.get<std::shared_ptr<LocalLuaEntity>>("object");
            sol::optional<sol::table> luaEntTable = core["entities"][object->id];
            if (!luaEntTable) return;

            sol::optional<sol::function> onDestruct = (*luaEntTable)["on_destroy"];
            if (onDestruct) (*onDestruct)();

            core["entities"][object->id] = sol::nil;
            world.dimension.removeLocalEntity(object);
        });
    }

    static void remove_entity_s(sol::state& lua, sol::table& core, ServerDefs& defs, ServerWorld& world) {
        core.set_function("remove_entity", [&](sol::table entity) {

//            sptr<LocalLuaEntity> object = entity.get<sptr<LocalLuaEntity>>("object");
//            sol::optional<sol::table> luaEntTable = core["entities"][object->id];
//            if (!luaEntTable) return;
//
//            sol::optional<sol::function> onDestruct = (*luaEntTable)["on_destroy"];
//            if (onDestruct) (*onDestruct)();
//
//            core["entities"][object->id] = sol::nil;
//            world.dimension.removeLocalEntity(object);
        });
    }
}