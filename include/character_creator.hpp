#pragma once
#ifndef character_creator_hpp
#define character_creator_hpp

#include "entities.hpp"

namespace CharacterCreator
{
    constexpr uint32_t STARTING_ATTRIBUTE_POINTS = 2;
    constexpr uint32_t STARTING_SPECIALTY_POINTS = 2;

    const Inventory basicInventory{ {}, { { "Torso", 7 }, { "Legs", 8 } } };

    const std::vector<Race> startingRaces{
        Race::Human,
        Race::Repsoris,
        Race::Ursobac,
        Race::Rodanto,
        Race::Felidae,
        Race::Strigifor,
        Race::Vulpotis,
    };

    const std::vector<std::string> startingAbilities{
        "first_aid",
        "evasion",
        "knockdown",
        "life_drain",
        "magic_shield",
        "camouflage",
        "noxious_cloud",
        "earthquake",
        "hailstorm",
        "reanimate",
    };

    const std::vector<uint32_t> startingItems{ 0, 1, 2, 3, 4, 5, 6 };

    std::vector<Hero> createHeroes();

    Hero pickHeroRace(const std::string &name);

    const std::string pickStartingAbility();

    uint32_t pickStartingItem(const Attributes &attributes);
}  // namespace CharacterCreator

#endif