// InputMap.cpp
#include "InputMap.hpp"

std::unordered_map<std::string, SDL_Scancode> InputMap::bindings;

void InputMap::BindKey(const std::string& key, SDL_Scancode scancode) 
{
    bindings[key] = scancode;
}

bool InputMap::IsKeyPressed(const std::string& key) 
{
    SDL_Scancode scancode = bindings[key];
    const Uint8* state = SDL_GetKeyboardState(NULL);
    return state[scancode];
}

bool InputMap::IsKeyReleased(const std::string& key) 
{
    SDL_Scancode scancode = bindings[key];
    const Uint8* state = SDL_GetKeyboardState(NULL);
    return !state[scancode];
}

int InputMap::GetDirection(const std::string& negativeKey, const std::string& positiveKey)
{
    int negativePressed = IsKeyPressed(negativeKey) ? -1 : 0;
    int positivePressed = IsKeyPressed(positiveKey) ? 1 : 0;

    return negativePressed + positivePressed;
}

Vector2& InputMap::GetVector(const std::string& leftKey, const std::string& rightKey, const std::string& upKey, const std::string& downKey) 
{
    static Vector2 vector;
    vector.x = GetDirection(leftKey, rightKey);
    vector.y = GetDirection(upKey, downKey);
    return vector;
}
