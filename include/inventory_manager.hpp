#pragma once
#ifndef inventory_manager_hpp
#define inventory_manager_hpp

#include <array>
#include <string>
#include <vector>
#include <optional>

struct Hero;
struct Item;

namespace InventoryManager
{
    enum class EquipmentSlot
    {
        MainHand = 0,
        Offhand = 1,
        Torso = 2,
        Head = 3,
        Legs = 4,
        Gloves = 5
    };

    static constexpr std::array<const char *, 6> equipmentSlotNames{ "Main Hand", "Offhand", "Torso", "Head", "Legs", "Gloves" };

    void addToBackpack(Hero &hero, const uint32_t itemId);

    void removeFromBackpack(Hero &hero, const uint32_t itemId);

    void clearIfTwoHanded(Hero &hero);

    void replaceEquipped(Hero &hero, const uint32_t itemId, const EquipmentSlot slot);

    void unequipItem(Hero &hero, const EquipmentSlot slot);

    void equipItem(Hero &hero, const uint32_t itemId, const EquipmentSlot slot);

    const std::vector<uint32_t> equipableInHand(const Hero &hero, const EquipmentSlot slot);

    std::optional<std::string> getEquippedItemName(const Hero &hero, const EquipmentSlot slot);

    std::optional<EquipmentSlot> selectSlot(const Hero &hero);

    void selectEquipment(Hero &hero);

    void selectPartyEquipment(std::vector<Hero> &heroes);

    uint32_t getEquippedArmorValue(const Hero &hero);

    uint32_t getEquippedDamageValue(const Hero &hero);

    std::string getEquipmentSlotName(const EquipmentSlot eSlot);

    uint32_t getPrimaryAttributeValueFromHero(const Item &item, const Hero &hero);
}  // namespace InventoryManager

#endif