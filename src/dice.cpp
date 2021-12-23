#include "dice.hpp"

namespace Dice
{
    auto getGlobalRandom() -> std::default_random_engine&
    {
        static std::default_random_engine u{};
        return u;
    }

    void randomize()
    {
        static std::random_device rd{};
        getGlobalRandom().seed(rd());
    }

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
        return distr(getGlobalRandom());
    }
}  // namespace Dice