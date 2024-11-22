#pragma once
#include <SDL.h>
#include <iostream>
#include <unordered_map>
#include <functional>

class InputMap {
public:
    static void BindKey(const std::string& key, const SDL_Keycode& keycode);

    static bool IsKeyPressed(const std::string& key);
    static bool IsKeyReleased(const std::string& key);

private:
    static std::unordered_map<std::string, SDL_Keycode> bindings;
};