#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <cstdio>  
#include <bitset>
#include <array>

using EntityID = std::size_t;
const EntityID MAX_ENTITIES = 2048;

using ComponentID = std::size_t;
const ComponentID MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

#endif // !DEFINITIONS_HPP


