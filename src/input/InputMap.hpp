// InputMap.hpp
#pragma once

#include <unordered_map>
#include <string>
#include <SDL.h>
#include "../common/Utils.hpp"


class InputMap
{
public:
    static void BindKey(const std::string& key, SDL_Scancode scancode);
    static bool IsKeyPressed(const std::string& key);
    static bool IsKeyReleased(const std::string& key);
    static int GetDirection(const std::string& negativeKey, const std::string& positiveKey);
    static Vector2& GetVector(const std::string& leftKey, const std::string& rightKey, const std::string& upKey, const std::string& downKey);

    // Mouse input functions
    static bool MouseLeftPressed(SDL_Event event);
    static bool MouseMiddlePressed(SDL_Event event);
    static bool MouseRightPressed(SDL_Event event);

private:
    static std::unordered_map<std::string, SDL_Scancode> bindings;
};
