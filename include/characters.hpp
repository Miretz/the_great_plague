#pragma once
#ifndef characters_hpp
#define characters_hpp

#include <string>
#include <vector>

struct Hero;

namespace Characters
{
    inline constexpr uint32_t kVitalityPerPoint = 10;

    inline constexpr uint32_t kAttributePointsPerLevel = 2;
    inline constexpr uint32_t kSpecialtyPointsPerLevel = 2;
    inline constexpr uint32_t kHealthPerLevel = 5;

    inline constexpr uint32_t kAdjustmentBorderSize = 55;

    void levelUp(Hero &hero);

    void assignAttributePoints(Hero &hero);

    void assignSpecialtyPoints(Hero &hero);

    void recalculateHeroHealth(Hero &hero);

    auto isNameAlreadyInUse(const std::string &name, const std::vector<Hero> &heroes) -> bool;

    void takeDamage(Hero &hero, const uint32_t amount);

    void heal(Hero &hero, const uint32_t amount);
}  // namespace Characters

#endif