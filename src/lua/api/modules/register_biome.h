//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include "../../../util/Log.h"

namespace Api {
    static void register_biome(sol::state& lua, sol::table& core) {
        core["registered_biomes"] = lua.create_table();

        core.set_function("register_biome", [&](sol::this_environment env, sol::optional<std::string> identifier, sol::optional<sol::table> data) {
            if (!identifier || !identifier->length()) throw std::runtime_error("expected a string as the first argument.");
            if (!data) throw std::runtime_error("expected a table as the second argument.");

            auto modname = static_cast<sol::environment>(env).get<std::string>("_MODNAME");
            if (identifier->compare(0, modname.length() + 1, modname + ":")) throw std::runtime_error(
                        "identifier '" + *identifier + "' must begin with the calling mod's prefix, '" + modname + "'.");

            core["registered_biomes"][identifier] = data;
        });
    }
}