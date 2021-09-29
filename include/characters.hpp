#pragma once

#include "entities.hpp"

namespace Characters
{
    constexpr uint32_t STARTING_POINTS = 4;
    constexpr uint32_t STARTING_HEALTH = 100;

    const Inventory basicInventory{
        {},
        {{"Torso", 7}, {"Legs", 8}}};

    const std::vector<Race> startingRaces{
        Race::Human,
        Race::Repsoris,
        Race::Ursobac,
        Race::Rodanto,
        Race::Felidae,
        Race::Strigifor,
        Race::Vulpotis,
    };

    Hero pickHeroRace(std::string name);

    void assignAttributePoints(Hero &hero);

    std::vector<Hero> createHeroes();
}