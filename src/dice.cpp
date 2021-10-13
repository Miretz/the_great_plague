#include "dice.hpp"

namespace Dice
{
    std::random_device rd;
    std::mt19937 generator(rd());

    uint32_t rollDice(std::uniform_int_distribution<int> die)
    {
        return die(generator);
    }

    uint32_t randomSelection(uint32_t min, uint32_t max)
    {
        if (min == max)
        {
            return min;
        }
        std::uniform_int_distribution<> distr(min, max);
        return distr(generator);
    }
}  // namespace Dice