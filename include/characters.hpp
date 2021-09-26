#pragma once

#include "entities.hpp"

namespace Characters
{
    constexpr unsigned int STARTING_POINTS = 20;
    constexpr unsigned int STARTING_HEALTH = 100;

    const Item basicClothes{"Worn clothes", "Basic clothing", ItemType::Armor_Torso, {}, 0, 2};
    const Item basicShoes{"Worn boots", "Basic boots", ItemType::Armor_Torso, {}, 0, 2};

    const Inventory basicInventory{
        {},
        {{"Torso", basicClothes}, {"Legs", basicShoes}}};

    const std::vector<Hero> presets{
        {"War", STARTING_HEALTH, 0, STARTING_HEALTH, 1, 100, Controller::Player, {10, 4, 5, 1, 0}, {}, {}, basicInventory},
        {"Nimble", STARTING_HEALTH, 0, STARTING_HEALTH, 1, 100, Controller::Player, {4, 10, 3, 3, 0}, {}, {}, basicInventory},
        {"Mystic", STARTING_HEALTH, 0, STARTING_HEALTH, 1, 100, Controller::Player, {3, 3, 4, 10, 0}, {}, {}, basicInventory},
        {"Custom", STARTING_HEALTH, 0, STARTING_HEALTH, 1, 100, Controller::Player, {0, 0, 0, 0, 20}, {}, {}, basicInventory},
    };

    Hero pickHeroPreset();

    Attributes updateAttributes(Attributes attributes);

    std::vector<Hero> createHeroes();
}