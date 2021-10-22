#pragma once
#ifndef inventory_manager_hpp
#define inventory_manager_hpp

#include <array>
#include <optional>
#include <string>
#include <vector>

struct Hero;
struct Item;

namespace InventoryManager
{
    static constexpr auto kDualWieldPenalty = 0.25;
    static constexpr auto kEmptyHandsDamage = 5;

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

    auto equipableInHand(const Hero &hero, const EquipmentSlot slot) -> const std::vector<uint32_t>;

    auto getEquippedItemName(const Hero &hero, const EquipmentSlot slot) -> std::optional<std::string>;

    auto selectSlot(const Hero &hero) -> std::optional<EquipmentSlot>;

    void selectEquipment(Hero &hero);

    void selectPartyEquipment(std::vector<Hero> &heroes);

    auto getEquippedArmorValue(const Hero &hero) -> uint32_t;

    auto getEquippedDamageValue(const Hero &hero) -> uint32_t;

    auto getEquipmentSlotName(const EquipmentSlot eSlot) -> std::string;

    auto getPrimaryAttributeValueFromHero(const Item &item, const Hero &hero) -> uint32_t;
}  // namespace InventoryManager

#endif