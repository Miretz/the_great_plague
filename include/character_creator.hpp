#pragma once
#ifndef character_creator_hpp
#define character_creator_hpp

#include "entities.hpp"

namespace CharacterCreator
{
    constexpr uint32_t STARTING_ATTRIBUTE_POINTS = 2;
    constexpr uint32_t STARTING_SPECIALTY_POINTS = 2;

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

    const std::vector<uint32_t> startingAbilities{
        1, // First Aid
        2, // Evasion
        3, // Knockdown
        4, // Life Drain
        5, // Magic Shield
        8, // Camouflage
    };

    const std::vector<uint32_t> startingItems{0, 1, 2, 3, 4, 5, 6};

    std::vector<Hero> createHeroes();

    Hero pickHeroRace(const std::string &name);

    uint32_t pickStartingAbility();

    uint32_t pickStartingItem(Attributes &attributes);
}

#endif