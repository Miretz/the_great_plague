#include "inventory_manager.hpp"

#include <iterator>
#include <optional>
#include <stdexcept>

#include "dice.hpp"
#include "entities.hpp"
#include "utils.hpp"

namespace InventoryManager
{
    void addToBackpack(Hero &hero, const uint32_t itemId)
    {
        hero.inventory.backpack.push_back(itemId);
    }

    void removeFromBackpack(Hero &hero, const uint32_t itemId)
    {
        std::vector<uint32_t> newBackpack;
        bool found = false;

        for (auto backpackItem : hero.inventory.backpack)
        {
            if (!found && backpackItem == itemId)
            {
                found = true;
                continue;
            }
            newBackpack.push_back(backpackItem);
        }
        hero.inventory.backpack = newBackpack;
    }

    void clearIfTwoHanded(Hero &hero)
    {
        const auto &mainHand = getEquipmentSlotName(EquipmentSlot::MainHand);
        const auto &offHand = getEquipmentSlotName(EquipmentSlot::Offhand);

        if (hero.inventory.equipped.find(mainHand) != hero.inventory.equipped.end())
        {
            const auto &equippedMainHandItem = g_AllItems[hero.inventory.equipped[mainHand]];
            if (equippedMainHandItem.type == ItemType::Melee_TwoHanded ||
                equippedMainHandItem.type == ItemType::Ranged_TwoHanded)
            {
                addToBackpack(hero, hero.inventory.equipped[mainHand]);
                hero.inventory.equipped.erase(mainHand);
                hero.inventory.equipped.erase(offHand);
            }
        }
    }

    void replaceEquipped(Hero &hero, const uint32_t itemId, const EquipmentSlot slot)
    {
        if (slot == EquipmentSlot::MainHand || slot == EquipmentSlot::Offhand)
        {
            clearIfTwoHanded(hero);
        }

        const auto &hand = getEquipmentSlotName(slot);
        if (hero.inventory.equipped.find(hand) != hero.inventory.equipped.end())
        {
            addToBackpack(hero, hero.inventory.equipped[hand]);
        }
        hero.inventory.equipped[hand] = itemId;

        const auto &item = g_AllItems[itemId];
        if (item.type == ItemType::Melee_TwoHanded || item.type == ItemType::Ranged_TwoHanded)
        {
            const auto &offHand = getEquipmentSlotName(EquipmentSlot::Offhand);
            if (hero.inventory.equipped.find(offHand) != hero.inventory.equipped.end())
            {
                addToBackpack(hero, hero.inventory.equipped[offHand]);
            }

            hero.inventory.equipped[offHand] = itemId;
        }

        removeFromBackpack(hero, itemId);
    }

    void unequipItem(Hero &hero, const EquipmentSlot slot)
    {
        if (slot == EquipmentSlot::MainHand || slot == EquipmentSlot::Offhand)
        {
            clearIfTwoHanded(hero);
        }

        const auto &slotName = getEquipmentSlotName(slot);
        if (hero.inventory.equipped.find(slotName) != hero.inventory.equipped.end())
        {
            addToBackpack(hero, hero.inventory.equipped[slotName]);
            hero.inventory.equipped.erase(slotName);
        }
    }

    void equipItem(Hero &hero, const uint32_t itemId, const EquipmentSlot slot)
    {
        // Can't equip these types
        const auto &item = g_AllItems[itemId];
        if (item.type == ItemType::Scroll || item.type == ItemType::Throwable || item.type == ItemType::Consumable)
        {
            addToBackpack(hero, itemId);
            return;
        }

        if (hero.attributes.strength >= item.requirements.strength &&
            hero.attributes.dexterity >= item.requirements.dexterity &&
            hero.attributes.constitution >= item.requirements.constitution &&
            hero.attributes.intelligence >= item.requirements.intelligence)
        {
            replaceEquipped(hero, itemId, slot);
        }
        else
        {
            // can't equip because of attributes - put back to the backpack
            addToBackpack(hero, itemId);
        }
    }

    auto equipableInHand(const Hero &hero, const EquipmentSlot slot) -> const std::vector<uint32_t>
    {
        std::vector<uint32_t> equipable;

        for (auto itemId : hero.inventory.backpack)
        {
            const auto itemType = g_AllItems[itemId].type;
            if (slot == EquipmentSlot::MainHand)
            {
                if (itemType == ItemType::Melee_OneHanded || itemType == ItemType::Melee_TwoHanded ||
                    itemType == ItemType::Ranged_OneHanded || itemType == ItemType::Ranged_TwoHanded)
                {
                    equipable.push_back(itemId);
                }
            }
            else if (slot == EquipmentSlot::Offhand)
            {
                if (itemType == ItemType::Shield || itemType == ItemType::Melee_OneHanded ||
                    itemType == ItemType::Ranged_OneHanded)
                {
                    equipable.push_back(itemId);
                }
            }
            else if (
                (slot == EquipmentSlot::Gloves && itemType == ItemType::Armor_Gloves) ||
                (slot == EquipmentSlot::Torso && itemType == ItemType::Armor_Torso) ||
                (slot == EquipmentSlot::Legs && itemType == ItemType::Armor_Legs) ||
                (slot == EquipmentSlot::Head && itemType == ItemType::Armor_Head))
            {
                equipable.push_back(itemId);
            }
        }

        return equipable;
    }

    auto getEquippedItemName(const Hero &hero, const EquipmentSlot slot) -> std::optional<std::string>
    {
        const auto &slotName = getEquipmentSlotName(slot);
        if (hero.inventory.equipped.find(slotName) != hero.inventory.equipped.end())
        {
            return g_AllItems[hero.inventory.equipped.at(slotName)].name;
        }

        return std::nullopt;
    }

    auto selectSlot(const Hero &hero) -> std::optional<EquipmentSlot>
    {
        const auto prompt = [&hero]()
        {
            Utils::printSpacedText("Manage Inventory");
            Utils::printBorder();
            Utils::printFullPhysicalDamage(hero);
            Utils::printHeroInventory(hero.inventory);
            Utils::printSpacedText("Pick a slot to edit:");
        };

        std::vector<std::string> slotMenu;
        std::copy(equipmentSlotNames.begin(), equipmentSlotNames.end(), std::back_inserter(slotMenu));
        slotMenu.emplace_back("Exit");

        const auto selection = Utils::pickOptionFromList(prompt, slotMenu);
        if (selection == slotMenu.size() - 1)
        {
            return std::nullopt;
        }
        const auto selectedSlot = static_cast<EquipmentSlot>(selection);

        Utils::printSpacedText(
            "Selected slot " + slotMenu[selection] +
            " (current: " + getEquippedItemName(hero, selectedSlot).value_or("Empty") + ")\n");

        return selectedSlot;
    }

    void selectPartyEquipment(std::vector<Hero> &heroes)
    {
        while (true)
        {
            std::vector<std::string> menu;
            std::transform(heroes.begin(), heroes.end(), std::back_inserter(menu), [](const auto &h) { return h.name; });
            menu.emplace_back("Print Character Sheet");
            menu.emplace_back("Exit");

            const auto selection =
                Utils::pickOptionFromList([]() { Utils::printBorderedText("Manage inventory of:"); }, menu);

            if (selection < menu.size() - 2)
            {
                auto &hero = heroes[selection];
                selectEquipment(hero);
            }
            else if (selection == menu.size() - 2)
            {
                Utils::clearScreen();
                Utils::printListOfHeroes(heroes);
                Utils::pressEnterToContinue();
            }
            else
            {
                break;
            }
        }
    }

    void selectEquipment(Hero &hero)
    {
        while (true)
        {
            Utils::clearScreen();

            const auto selectedSlotMaybe = selectSlot(hero);
            if (!selectedSlotMaybe.has_value())
            {
                break;
            }
            const auto selectedSlot = selectedSlotMaybe.value();

            // check what we can equip
            const auto listOfEquipable = equipableInHand(hero, selectedSlot);

            bool canUnequip = getEquippedItemName(hero, selectedSlot).has_value();
            bool canEquip = listOfEquipable.size() > 0;

            static constexpr auto unequipActionName = "Unequip current item";
            static constexpr auto equipActionName = "Equip item";

            std::vector<std::string> actions;
            if (canUnequip)
            {
                actions.emplace_back(unequipActionName);
            }
            if (canEquip)
            {
                actions.emplace_back(equipActionName);
            }
            actions.emplace_back("Back");

            const auto &slotName = getEquipmentSlotName(selectedSlot);

            const auto prompt = [hero, slotName]()
            {
                Utils::printSpacedText("Manage Inventory");
                Utils::printBorder();
                Utils::printFullPhysicalDamage(hero);
                Utils::printHeroInventory(hero.inventory);
                Utils::printSpacedText("Manage slot " + slotName + ": ");
            };

            const auto &selectedAction = actions[Utils::pickOptionFromList(prompt, actions)];

            // execute the action

            if (selectedAction == unequipActionName)
            {
                unequipItem(hero, selectedSlot);
            }
            else if (selectedAction == equipActionName)
            {
                Utils::clearScreen();

                std::vector<std::string> chooseWeapon;
                std::transform(
                    listOfEquipable.begin(),
                    listOfEquipable.end(),
                    std::back_inserter(chooseWeapon),
                    [](const auto &e) { return Utils::getItemString(e, true); });

                const auto pickItemPrompt = []() { Utils::printBorderedText("Select equipment:"); };
                const auto itemSelection = Utils::pickOptionFromList(pickItemPrompt, chooseWeapon);
                const auto newItem = listOfEquipable[itemSelection];

                equipItem(hero, newItem, selectedSlot);
            }
            else if (selectedAction == "Back")
            {
                continue;
            }
        }
    }

    auto getEquippedArmorValue(const Hero &hero) -> uint32_t
    {
        uint32_t value = 0;

        std::vector<EquipmentSlot> armorSlots{
            EquipmentSlot::Head,
            EquipmentSlot::Torso,
            EquipmentSlot::Gloves,
            EquipmentSlot::Legs,
        };

        for (auto slot : armorSlots)
        {
            const auto &slotName = getEquipmentSlotName(slot);
            if (hero.inventory.equipped.find(slotName) != hero.inventory.equipped.end())
            {
                value += g_AllItems[hero.inventory.equipped.at(slotName)].armor;
            }
        }

        return value;
    }

    auto getEquippedDamageValue(const Hero &hero) -> uint32_t
    {
        uint32_t value = 0;

        // handle main hand
        const auto &mainHandSlot = getEquipmentSlotName(EquipmentSlot::MainHand);
        const auto &offHandSlot = getEquipmentSlotName(EquipmentSlot::Offhand);

        // check which slots are equipped
        const bool isMainHand = hero.inventory.equipped.find(mainHandSlot) != hero.inventory.equipped.end();
        const bool isOffHand = hero.inventory.equipped.find(offHandSlot) != hero.inventory.equipped.end();

        // both hands are equipped
        if (isMainHand && isOffHand)
        {
            const auto &mainItem = g_AllItems[hero.inventory.equipped.at(mainHandSlot)];
            const auto &offItem = g_AllItems[hero.inventory.equipped.at(offHandSlot)];

            // if it's a two handed melee weapon
            if (mainItem.type == ItemType::Melee_TwoHanded)
            {
                value += mainItem.damage;
                value += getPrimaryAttributeValueFromHero(mainItem, hero);
                value += hero.specialties.twoHanded;
            }
            // if it's a two handed ranged weapon
            else if (mainItem.type == ItemType::Ranged_TwoHanded)
            {
                value += mainItem.damage;
                value += getPrimaryAttributeValueFromHero(mainItem, hero);
                value += hero.specialties.ranged;
            }
            // if it's dual wielding
            else if (mainItem.type == offItem.type)
            {
                value += mainItem.damage;
                value += getPrimaryAttributeValueFromHero(mainItem, hero);

                // dual wielding penalty on the offhand weapon
                static constexpr auto division = 2.0;
                value += static_cast<uint32_t>(std::ceil(
                    static_cast<double>(offItem.damage) / division +
                    kDualWieldPenalty * static_cast<double>(hero.specialties.dualWielding)));
            }
            // sword + shield and other combinations
            else
            {
                value += mainItem.damage;
                value += getPrimaryAttributeValueFromHero(mainItem, hero);
                value += hero.specialties.oneHanded;

                value += offItem.damage;
            }
        }
        // only main hand is equipped
        else if (isMainHand)
        {
            const auto &mainItem = g_AllItems[hero.inventory.equipped.at(mainHandSlot)];

            value += mainItem.damage;
            value += getPrimaryAttributeValueFromHero(mainItem, hero);
            value += hero.specialties.oneHanded;
        }
        // only offhand is eqipped
        else if (isOffHand)
        {
            const auto &offItem = g_AllItems[hero.inventory.equipped.at(offHandSlot)];

            value += offItem.damage;
            value += getPrimaryAttributeValueFromHero(offItem, hero);
            value += hero.specialties.oneHanded;
        }

        // you can still do some damage with empty hands
        if (value == 0)
        {
            value += kEmptyHandsDamage;
        }

        return value;
    }

    auto getEquipmentSlotName(const EquipmentSlot eSlot) -> std::string
    {
        switch (eSlot)
        {
            case EquipmentSlot::MainHand: return equipmentSlotNames[0];  // NOLINT
            case EquipmentSlot::Offhand: return equipmentSlotNames[1];   // NOLINT
            case EquipmentSlot::Torso: return equipmentSlotNames[2];     // NOLINT
            case EquipmentSlot::Head: return equipmentSlotNames[3];      // NOLINT
            case EquipmentSlot::Legs: return equipmentSlotNames[4];      // NOLINT
            case EquipmentSlot::Gloves: return equipmentSlotNames[5];    // NOLINT
        }

        throw std::invalid_argument("Recieved a wrong EquipmentSlot value");
    }

    auto getPrimaryAttributeValueFromHero(const Item &item, const Hero &hero) -> uint32_t
    {
        static constexpr uint32_t scaleDownBy = 9;

        switch (item.primaryAttribute)
        {
            case PrimaryAttribute::Strength: return hero.attributes.strength - scaleDownBy;
            case PrimaryAttribute::Dexterity: return hero.attributes.dexterity - scaleDownBy;
            case PrimaryAttribute::Intelligence: return hero.attributes.intelligence - scaleDownBy;
            case PrimaryAttribute::None:
            default: return 0;
        }
    }
}  // namespace InventoryManager