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

// Meter to pixels conversion
constexpr uint8_t METER = 64;

using EntityID = std::uint16_t;
constexpr std::uint16_t MAX_ENTITIES = std::numeric_limits<uint16_t>::max();

using ComponentID = std::uint8_t;
constexpr std::uint8_t MAX_COMPONENTS = 16;

using Signature = std::bitset<MAX_COMPONENTS>;

constexpr uint16_t NULL_INDEX = std::numeric_limits<uint16_t>::max();
constexpr size_t MAX_PAGE_SIZE = 512;
