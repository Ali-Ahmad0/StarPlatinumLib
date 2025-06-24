#include "InputMap.hpp"
#include "../common/Utils.hpp"

std::unordered_map<std::string, SDL_Scancode> InputMap::bindings;

void InputMap::BindKey(const std::string& key, SDL_Scancode scancode)
{
    bindings[key] = scancode;
}

// Keyboard input
bool InputMap::IsKeyPressed(const std::string& key)
{
    SDL_Scancode scancode = bindings[key];
    const Uint8* state = SDL_GetKeyboardState(NULL);
    return state[scancode];
}

bool InputMap::IsKeyReleased(const std::string& key)
{
    return !IsKeyPressed(key);
}

int InputMap::GetDirection(const std::string& neg, const std::string& pos)
{
    int negPressed = IsKeyPressed(neg) ? -1 : 0;
    int posPressed = IsKeyPressed(pos) ? 1 : 0;

    return negPressed + posPressed;
}

void InputMap::GetVector(
    const std::string& negX, const std::string& posX, 
    const std::string& negY, const std::string& posY, Vector2& outVector)
{
    outVector.x = (float)GetDirection(negX, posX);
    outVector.y = (float)GetDirection(negY, posY);   
}

// Mouse input
bool InputMap::MouseLeftPressed(SDL_Event event)
{
    return event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT;
}

bool InputMap::MouseMiddlePressed(SDL_Event event)
{
    return event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_MIDDLE;
}

bool InputMap::MouseRightPressed(SDL_Event event)
{
    return event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT;
}
