#pragma once
#ifndef inventory_manager_hpp
#define inventory_manager_hpp

#include <vector>
#include <string>

struct Hero;

namespace InventoryManager
{
    enum class EquipmentSlot
    {
        MainHand,
        Offhand,
        Torso,
        Head,
        Legs,
        Gloves
    };

    const std::vector<std::string> equipmentSlotNames{"Main Hand", "Offhand", "Torso", "Head", "Legs", "Gloves"};

    void addToBackpack(Hero &hero, uint32_t itemId);

    void removeFromBackpack(Hero &hero, uint32_t itemId);

    void clearIfTwoHanded(Hero &hero);

    void replaceEquipped(Hero &hero, uint32_t itemId, EquipmentSlot slot);

    void unequipItem(Hero &hero, EquipmentSlot slot);

    void equipItem(Hero &hero, uint32_t itemId, EquipmentSlot slot);

    std::vector<uint32_t> equipableInHand(Hero &hero, EquipmentSlot slot);

    std::string getEquippedItemName(Hero &hero, EquipmentSlot slot);

    std::pair<bool, EquipmentSlot> selectSlot(Hero &hero);

    void selectEquipment(Hero &hero);

    uint32_t getEquippedArmorValue(Hero &hero);

    std::string getEquipmentSlotName(EquipmentSlot eSlot);
}

#endif