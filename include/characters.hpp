#pragma once

#include "entities.hpp"

namespace Characters
{
    constexpr uint32_t STARTING_POINTS = 4;
    constexpr uint32_t STARTING_HEALTH = 100;

    const Item basicClothes{"Worn clothes", "Basic clothing", ItemType::Armor_Torso, {}, 0, 2};
    const Item basicShoes{"Worn boots", "Basic boots", ItemType::Armor_Legs, {}, 0, 2};

    const Inventory basicInventory{
        {},
        {{"Torso", basicClothes}, {"Legs", basicShoes}}};

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