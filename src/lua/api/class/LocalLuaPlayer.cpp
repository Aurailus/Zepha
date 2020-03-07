//
// Created by aurailus on 2019-10-19.
//

#include "LocalLuaPlayer.h"

void LocalLuaPlayer::set_pos(const sol::table &pos) {
    player.setPos({pos[1], pos[2], pos[3]});
}

sol::table LocalLuaPlayer::get_pos(sol::this_state s) {
    glm::vec3 pos = player.getPos();
    return LuaParser::luaVec(sol::state_view(s), pos);
}

sol::table LocalLuaPlayer::get_block_pos(sol::this_state s) {
    glm::vec3 pos = glm::floor(player.getPos());
    return LuaParser::luaVec(sol::state_view(s), pos);
}

void LocalLuaPlayer::set_vel(const sol::table &vel) {
    player.setVel({vel[1], vel[2], vel[3]});
}

sol::table LocalLuaPlayer::get_vel(sol::this_state s) {
    glm::vec3 vel = player.getVel();
    return LuaParser::luaVec(sol::state_view(s), vel);
}

void LocalLuaPlayer::set_look_yaw(float rot) {
    player.setYaw(rot);
}

float LocalLuaPlayer::get_look_yaw() {
    return player.getYaw();
}

void LocalLuaPlayer::set_look_pitch(float rot) {
    player.setPitch(rot);
}

float LocalLuaPlayer::get_look_pitch() {
    return player.getPitch();
}

std::string LocalLuaPlayer::get_menu_state() {
    return player.getMenuState();
}

void LocalLuaPlayer::open_menu(sol::this_state s, std::string menu, sol::optional<sol::table> callbacks) {
    if (callbacks) {
        std::map<std::string, GuiBuilder::ComponentCallbacks> callbackMap;

        for (auto& pair : *callbacks)  {
            if (!pair.first.is<std::string>() || !pair.second.is<sol::table>()) continue;
            std::string identifier = pair.first.as<std::string>();
            sol::table callbacks = pair.second.as<sol::table>();
            GuiBuilder::ComponentCallbacks componentCallbacks {};

            auto left  = callbacks.get<sol::optional<sol::function>>("left");
            auto right = callbacks.get<sol::optional<sol::function>>("right");
            auto hover = callbacks.get<sol::optional<sol::function>>("hover");

            if (left) componentCallbacks.left = [=](bool down, glm::ivec2 pos) {
                    (*left)(down, sol::state_view(s).create_table_with("x", pos.x, "y", pos.y)); };

            if (right) componentCallbacks.right = [=](bool down, glm::ivec2 pos) {
                    (*right)(down, sol::state_view(s).create_table_with("x", pos.x, "y", pos.y)); };

            if (hover) componentCallbacks.hover = [=](bool down, glm::ivec2 pos) {
                    (*hover)(down, sol::state_view(s).create_table_with("x", pos.x, "y", pos.y)); };

            callbackMap.emplace(identifier, componentCallbacks);
        }

        player.setMenu(menu, callbackMap);
    }
    else player.setMenu(menu, {});
}

void LocalLuaPlayer::close_menu() {
    player.closeMenu();
}

LocalLuaInventory LocalLuaPlayer::get_inventory() {
    return LocalLuaInventory(player.getInventory());
}

void LocalLuaPlayer::set_selected_block(std::string block) {
    player.setActiveBlock(block);
}

void LocalLuaPlayer::set_flying(bool shouldFly) {
    player.setFlying(shouldFly);
}

bool LocalLuaPlayer::get_flying() {
    return player.isFlying();
}
