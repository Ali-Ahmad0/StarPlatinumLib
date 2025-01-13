#pragma once

#include <cstddef>
#include <limits>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <iostream>  
#include <bitset>
#include <queue>
#include <array>
#include <set>
#include <stdexcept>

using EntityID = std::uint16_t;
constexpr std::uint16_t MAX_ENTITIES = 10240;

using ComponentID = std::uint8_t;
constexpr std::uint8_t MAX_COMPONENTS = 16;

using ComponentSignature = std::bitset<MAX_COMPONENTS>;