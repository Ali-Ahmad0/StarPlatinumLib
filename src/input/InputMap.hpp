#pragma once
#include <SDL.h>
#include <iostream>
#include <unordered_map>
#include <functional>

class InputMap {
public:
    static void BindKey(const std::string& key, const SDL_Keycode& keycode) {
        bindings[key] = keycode;
    }

    static bool IsKeyJustPressed(const std::string& key, SDL_Event event) {
        auto it = bindings.find(key);
        if (it != bindings.end()) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == it->second) {
                return true;
            }
        }
        return false;
    }


    static bool IsKeyJustReleased(const std::string& key, SDL_Event event) {
        auto it = bindings.find(key);
        if (it != bindings.end()) {
            if (event.type == SDL_KEYUP && event.key.keysym.sym == it->second) {
                return true;
            }
        }
        return false;
    }

    static bool IsKeyReleased(const std::string& key) {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        auto it = bindings.find(key);
        if (it != bindings.end()) {
            SDL_Scancode scancode = SDL_GetScancodeFromKey(it->second);
            if (state[scancode] == 0) {
                return true;
            }
        }
        return false;
    }

    static bool IsKeyPressed(const std::string& key) {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        auto it = bindings.find(key);
        if (it != bindings.end()) {
            SDL_Scancode scancode = SDL_GetScancodeFromKey(it->second);
            if (state[scancode] == 1) {
                return true;
            }
        }
        return false;
    }




private:
    static std::unordered_map<std::string, SDL_Keycode> bindings;
};