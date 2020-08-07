#pragma once

#include <bitset>
#include <cassert>

// Aliases
using Entity = std::uint32_t;
using ComponentType = std::uint8_t;

// Constants
constexpr Entity MAX_ENTITIES = 5000;
constexpr ComponentType MAX_COMPONENTS = 32;

// More aliases
using Signature = std::bitset<MAX_COMPONENTS>;
