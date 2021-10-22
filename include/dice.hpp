#pragma once
#ifndef dice_hpp
#define dice_hpp

#include <algorithm>
#include <cstdint>
#include <functional>
#include <random>

namespace Dice
{
    // used for weapon attacks, healing, spells, etc.
    constexpr uint32_t D4 = 4;
    constexpr uint32_t D6 = 6;
    constexpr uint32_t D8 = 8;
    constexpr uint32_t D10 = 10;
    constexpr uint32_t D12 = 12;

    // used for checking the success of an action
    constexpr uint32_t D20 = 20;

    auto rollDice(uint32_t die) -> uint32_t;

    auto randomSelection(uint32_t min, uint32_t max) -> uint32_t;
}  // namespace Dice

#endif
