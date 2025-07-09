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
    static int GetDirection(const std::string& neg, const std::string& pos);
    static void GetVector(
        const std::string& negX, const std::string& posX, 
        const std::string& negY, const std::string& posY, Vector2& outVector);

    static bool MouseLeftPressed(SDL_Event event);
    static bool MouseMiddlePressed(SDL_Event event);
    static bool MouseRightPressed(SDL_Event event);

    static void GetMousePositionWindow(Vector2& outVector);
    static void GetMousePositionGlobal(Vector2& outVector);

private:
    static std::unordered_map<std::string, SDL_Scancode> bindings;
};
