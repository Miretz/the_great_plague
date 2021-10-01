#pragma once
#ifndef characters_hpp
#define characters_hpp

#include <string>
#include <vector>

struct Hero;

namespace Characters
{
    constexpr uint32_t HEALTH_PER_VITALITY_POINT = 10;
    constexpr uint32_t POINTS_PER_LEVEL = 2;

    void levelUp(Hero &hero);

    void assignAttributePoints(Hero &hero);

    void recalculateHeroHealth(Hero &hero);

    bool isNameAlreadyInUse(const std::string &name, const std::vector<Hero> &heroes);

    void takeDamage(Hero &hero, const uint32_t amount);
}

#endif