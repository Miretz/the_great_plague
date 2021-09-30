#pragma once
#ifndef characters_hpp
#define characters_hpp

#include <string>
#include <vector>

struct Hero;

namespace Characters
{
    constexpr uint32_t HEALTH_PER_VITALITY_POINT = 10;

    void assignAttributePoints(Hero &hero);

    void recalculateHeroHealth(Hero &hero);

    bool isNameAlreadyInUse(std::string name, const std::vector<Hero> &heroes);
}

#endif