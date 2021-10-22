#pragma once
#ifndef character_creator_hpp
#define character_creator_hpp

#include <array>

#include "entities.hpp"

namespace CharacterCreator
{
    static constexpr uint32_t kStartingAttributePoints = 2;
    static constexpr uint32_t kStartingSpecialtyPoints = 2;

    const Inventory basicInventory{ {}, { { "Torso", 7 }, { "Legs", 8 } } };

    static constexpr std::array<Race, 7> startingRaces{
        Race::Human,
        Race::Repsoris,
        Race::Ursobac,
        Race::Rodanto,
        Race::Felidae,
        Race::Strigifor,
        Race::Vulpotis,
    };

    static constexpr std::array<const char *, 10> startingAbilities{
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

    static constexpr std::array<uint32_t, 7> startingItems{ 0, 1, 2, 3, 4, 5, 6 };

    auto createHeroes() -> std::vector<Hero>;

    auto pickHeroRace(const std::string &name) -> Hero;

    auto pickStartingAbility() -> const std::string;

    auto pickStartingItem(const Attributes &attributes) -> uint32_t;
}  // namespace CharacterCreator

#endif