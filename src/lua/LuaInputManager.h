//
// Created by aurailus on 14/10/19.
//

#pragma once

#include <sol2/sol.hpp>

class LuaInputManager {
public:
    LuaInputManager();

    void update(bool* keys);
    void triggerKeybinds();

    void bindOnDown(unsigned short key, const sol::function& cb);
    void bindOnUp(unsigned short key, const sol::function& cb);
private:
    bool keysDown[1024] {};
    bool keysPressed[1024] {};
    bool keysReleased[1024] {};

    std::array<std::vector<sol::function>, 1024> callbacksDown {};
    std::array<std::vector<sol::function>, 1024> callbacksUp {};
};