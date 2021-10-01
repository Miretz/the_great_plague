#pragma once
#ifndef dice_hpp
#define dice_hpp

#include <algorithm>
#include <random>
#include <functional>

namespace Dice
{
    std::random_device rd;
    std::mt19937 generator(rd());

    // used for weapon attacks, healing, spells, etc.
    std::uniform_int_distribution<int> D4(1, 4);
    std::uniform_int_distribution<int> D6(1, 6);
    std::uniform_int_distribution<int> D8(1, 8);
    std::uniform_int_distribution<int> D10(1, 10);
    std::uniform_int_distribution<int> D12(1, 12);

    // used for checking the success of an action
    std::uniform_int_distribution<int> D20(1, 20);

    /*
    Usually in D&D you roll D20 first and then the others for damage amount
    This might need some tweaking later
    */
    auto rollDiceD4 = std::bind(D4, generator);
    auto rollDiceD6 = std::bind(D6, generator);
    auto rollDiceD8 = std::bind(D8, generator);
    auto rollDiceD10 = std::bind(D10, generator);
    auto rollDiceD12 = std::bind(D12, generator);

    auto rollDiceD20 = std::bind(D20, generator);

    uint32_t randomSelection(uint32_t min, uint32_t max)
    {
        std::uniform_int_distribution<> distr(min, max);
        return distr(generator);
    }

}

#endif
