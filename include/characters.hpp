#pragma once

#include "entities.hpp"

namespace Characters
{
    constexpr unsigned int STARTING_POINTS = 4;
    constexpr unsigned int STARTING_HEALTH = 100;

    const Item basicClothes{"Worn clothes", "Basic clothing", ItemType::Armor_Torso, {}, 0, 2};
    const Item basicShoes{"Worn boots", "Basic boots", ItemType::Armor_Legs, {}, 0, 2};

    const Inventory basicInventory{
        {},
        {{"Torso", basicClothes}, {"Legs", basicShoes}}};

    const std::vector<RaceDetail> startingRaces{
        {Race::Repsoris, "Repsoris", "Identified by their reptilian features and hatred for cold.", {8, 6, 2, 1}},
        {Race::Ursobac, "Ursobac", "Their large intimidating physique makes them valuable protectors.", {12, 1, 6, 1}},
        {Race::Rodanto, "Rodanto", "They are especially proud of their prominent incisors, although they like to lurk in the shadows.", {1, 14, 2, 3}},
        {Race::Felidae, "Felidae", "Their grace and finesse is matched by none. Some believe that they have more than one life.", {1, 11, 4, 4}},
        {Race::Strigifor, "Strigifor", "Their huge eyes are full of wisdom and understanding, but the feathery crown on their head provides little protection.", {1, 1, 1, 15}},
        {Race::Vulpotis, "Vulpotis", "Slick, cunning and opportunistic. They hide a lot of wits under their dense fur.", {1, 4, 2, 13}},
    };

    Hero pickHeroRace(std::string name);

    void assignAttributePoints(Hero &hero);

    std::vector<Hero> createHeroes();
}