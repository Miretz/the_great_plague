#pragma once

#include "entities.hpp"

namespace InventoryManager
{

    const std::vector<Item> startingItems{
        {"Dagger", "Knife you stole from your mom's kitchen", ItemType::Dual_Wielding, {0, 0, 0, 0}, 3, 0},
        {"Short sword", "Simple short sword that anyone can use", ItemType::Melee_OneHanded, {0, 0, 0, 0}, 5, 0},
        {"Short bow", "Basic wooden bow", ItemType::Ranged_TwoHanded, {0, 0, 0, 0}, 5, 0},
        {"Wand", "A simple tree branch", ItemType::Ranged_OneHanded, {0, 0, 0, 0}, 5, 0},
        {"Long sword", "A long and heavy two-handed sword", ItemType::Melee_TwoHanded, {5, 0, 0, 0}, 8, 0},
        {"Long bow", "Long bow", ItemType::Ranged_TwoHanded, {0, 5, 0, 0}, 8, 0},
        {"Quarterstaff", "A solid broomstick", ItemType::Melee_TwoHanded, {0, 0, 0, 5}, 8, 0},
    };

    Item pickStartingItem(Attributes &attributes);

    void addToBackpack(Hero &hero, Item item);

    void removeFromBackpack(Hero &hero, Item item);

    void clearIfTwoHanded(Hero &hero);

    void replaceEquipped(Hero &hero, Item item, EquipmentSlot slot);

    void unequipItem(Hero &hero, EquipmentSlot slot);

    void equipItem(Hero &hero, Item item, EquipmentSlot slot);

    std::vector<Item> equipableInHand(const Hero &hero, EquipmentSlot slot);

    std::string getEquippedItemName(const Hero &hero, EquipmentSlot slot);

    std::pair<bool, EquipmentSlot> selectSlot(const Hero &hero);

    void selectEquipment(Hero &hero);

}