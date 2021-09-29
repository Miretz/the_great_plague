#pragma once

#include "entities.hpp"

namespace InventoryManager
{
    const std::vector<uint32_t> startingItems{0,1,2,3,4,5,6};

    uint32_t pickStartingItem(Attributes &attributes);

    void addToBackpack(Hero &hero, uint32_t itemId);

    void removeFromBackpack(Hero &hero, uint32_t itemId);

    void clearIfTwoHanded(Hero &hero);

    void replaceEquipped(Hero &hero, uint32_t itemId, EquipmentSlot slot);

    void unequipItem(Hero &hero, EquipmentSlot slot);

    void equipItem(Hero &hero, uint32_t itemId, EquipmentSlot slot);

    std::vector<uint32_t> equipableInHand(const Hero &hero, EquipmentSlot slot);

    std::string getEquippedItemName(const Hero &hero, EquipmentSlot slot);

    std::pair<bool, EquipmentSlot> selectSlot(const Hero &hero);

    void selectEquipment(Hero &hero);

    uint32_t getEquippedArmorValue(const Hero &hero);
}