#pragma once
#ifndef dice_hpp
#define dice_hpp

#include <algorithm>
#include <random>
#include <functional>

namespace Dice
{
    // used for weapon attacks, healing, spells, etc.
    const std::uniform_int_distribution<int> D4(1, 4);
    const std::uniform_int_distribution<int> D6(1, 6);
    const std::uniform_int_distribution<int> D8(1, 8);
    const std::uniform_int_distribution<int> D10(1, 10);
    const std::uniform_int_distribution<int> D12(1, 12);

    // used for checking the success of an action
    const std::uniform_int_distribution<int> D20(1, 20);

    uint32_t rollDice(std::uniform_int_distribution<int> die);

    uint32_t randomSelection(uint32_t min, uint32_t max);
}

#endif
