#pragma once
#ifndef character_creator_hpp
#define character_creator_hpp

#include <array>

#include "entities.hpp"

namespace CharacterCreator
{
    inline constexpr uint32_t kStartingAttributePoints = 2;
    inline constexpr uint32_t kStartingSpecialtyPoints = 2;

    inline const Inventory basicInventory{ {}, { { "Torso", 7 }, { "Legs", 8 } } };

    inline constexpr std::array<Race, 7> startingRaces{
        Race::Human,
        Race::Repsoris,
        Race::Ursobac,
        Race::Rodanto,
        Race::Felidae,
        Race::Strigifor,
        Race::Vulpotis,
    };

    inline constexpr std::array<const char *, 10> startingAbilities{
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

    inline constexpr std::array<uint32_t, 7> startingItems{ 0, 1, 2, 3, 4, 5, 6 };

    auto createHeroes() -> std::vector<Hero>;

    auto pickHeroRace(const std::string &name) -> Hero;

    auto pickStartingAbility() -> const std::string;

    auto pickStartingItem(const Attributes &attributes) -> uint32_t;
}  // namespace CharacterCreator

#endif