#include "dice.hpp"

namespace Dice
{
    static std::random_device rd;         // NOLINT
    static std::mt19937 generator(rd());  // NOLINT

    auto rollDice(uint32_t die) -> uint32_t
    {
        return randomSelection(1, die);
    }

    auto randomSelection(uint32_t min, uint32_t max) -> uint32_t
    {
        if (min == max)
        {
            return min;
        }

        std::uniform_int_distribution<uint32_t> distr(min, max);
        return distr(generator);
    }
}  // namespace Dice