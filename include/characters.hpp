#pragma once
#ifndef characters_hpp
#define characters_hpp

#include <string>
#include <vector>

struct Hero;

namespace Characters
{
    static constexpr uint32_t HEALTH_PER_VITALITY_POINT = 10;

    static constexpr uint32_t ATTRIBUTE_POINTS_PER_LEVEL = 2;
    static constexpr uint32_t SPECIALTY_POINTS_PER_LEVEL = 2;
    static constexpr uint32_t HEALTH_PER_LEVEL = 5;

    void levelUp(Hero &hero);

    void assignAttributePoints(Hero &hero);

    void assignSpecialtyPoints(Hero &hero);

    void recalculateHeroHealth(Hero &hero);

    bool isNameAlreadyInUse(const std::string &name, const std::vector<Hero> &heroes);

    void takeDamage(Hero &hero, const uint32_t amount);

    void heal(Hero &hero, const uint32_t amount);
}  // namespace Characters

#endif