#pragma once

#include <iostream>
#include <SDL.h>
#include <unordered_map>
#include "../common/Utils.hpp"

class InputMap 
{
public:
    static void BindKey(const std::string& key, SDL_Scancode keycode);

    static bool IsKeyReleased(const std::string& key);
    static bool IsKeyPressed(const std::string& key);

private:
    static std::unordered_map<std::string, SDL_Scancode> bindings;
};
