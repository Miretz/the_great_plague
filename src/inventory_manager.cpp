#include "inventory_manager.hpp"

#include "entities.hpp"
#include "utils.hpp"
#include "dice.hpp"

#include <iostream>

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
        auto mainHand = getEquipmentSlotName(EquipmentSlot::MainHand);
        auto offHand = getEquipmentSlotName(EquipmentSlot::Offhand);

        if (hero.inventory.equipped.find(mainHand) != hero.inventory.equipped.end())
        {
            auto equippedMainHandItem = g_AllItems[hero.inventory.equipped[mainHand]];
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

        auto hand = getEquipmentSlotName(slot);
        if (hero.inventory.equipped.find(hand) != hero.inventory.equipped.end())
        {
            addToBackpack(hero, hero.inventory.equipped[hand]);
        }
        hero.inventory.equipped[hand] = itemId;

        auto item = g_AllItems[itemId];
        if (item.type == ItemType::Melee_TwoHanded || item.type == ItemType::Ranged_TwoHanded)
        {
            auto offHand = getEquipmentSlotName(EquipmentSlot::Offhand);
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

        auto slotName = getEquipmentSlotName(slot);
        if (hero.inventory.equipped.find(slotName) != hero.inventory.equipped.end())
        {
            addToBackpack(hero, hero.inventory.equipped[slotName]);
            hero.inventory.equipped.erase(slotName);
        }
    }

    void equipItem(Hero &hero, const uint32_t itemId, const EquipmentSlot slot)
    {
        // Can't equip these types
        auto item = g_AllItems[itemId];
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

    const std::vector<uint32_t> equipableInHand(const Hero &hero, const EquipmentSlot slot)
    {
        std::vector<uint32_t> equipable;

        for (auto itemId : hero.inventory.backpack)
        {
            auto item = g_AllItems[itemId];
            if (slot == EquipmentSlot::MainHand)
            {
                if (item.type == ItemType::Melee_OneHanded ||
                    item.type == ItemType::Melee_TwoHanded ||
                    item.type == ItemType::Ranged_OneHanded ||
                    item.type == ItemType::Ranged_TwoHanded)
                {
                    equipable.push_back(itemId);
                }
            }
            else if (slot == EquipmentSlot::Offhand)
            {
                if (item.type == ItemType::Shield ||
                    item.type == ItemType::Melee_OneHanded ||
                    item.type == ItemType::Ranged_OneHanded)
                {
                    equipable.push_back(itemId);
                }
            }
            else if (slot == EquipmentSlot::Gloves && item.type == ItemType::Armor_Gloves)
            {
                equipable.push_back(itemId);
            }
            else if (slot == EquipmentSlot::Torso && item.type == ItemType::Armor_Torso)
            {
                equipable.push_back(itemId);
            }
            else if (slot == EquipmentSlot::Legs && item.type == ItemType::Armor_Legs)
            {
                equipable.push_back(itemId);
            }
            else if (slot == EquipmentSlot::Head && item.type == ItemType::Armor_Head)
            {
                equipable.push_back(itemId);
            }
        }

        return equipable;
    }

    const std::string getEquippedItemName(const Hero &hero, const EquipmentSlot slot)
    {
        auto slotName = getEquipmentSlotName(slot);
        if (hero.inventory.equipped.find(slotName) != hero.inventory.equipped.end())
        {
            auto item = g_AllItems[hero.inventory.equipped.at(slotName)];
            return item.name;
        }

        return "Empty";
    }

    std::pair<bool, EquipmentSlot> selectSlot(const Hero &hero)
    {
        std::vector<EquipmentSlot> availableSlots{
            EquipmentSlot::Head,
            EquipmentSlot::Torso,
            EquipmentSlot::Gloves,
            EquipmentSlot::Legs,
            EquipmentSlot::MainHand,
            EquipmentSlot::Offhand,
        };

        auto heroEquipped = Utils::getEquippedItemsString(hero.inventory.equipped);
        auto heroBackpack = Utils::getBackpack(hero.inventory.backpack);
        auto fullDamage = Utils::getFullPhysicalDamage(hero);

        auto prompt = [hero, heroEquipped, heroBackpack, fullDamage]()
        {
            Utils::printHeroHeader(hero.name, hero.level);
            std::cout << "|"
                      << "Inventory\n";
            Utils::printBorder(130);
            std::cout << heroEquipped;
            std::cout << heroBackpack;
            Utils::printBorder(130);
            std::cout << fullDamage;
            Utils::printBorder(130);
            std::cout << "\n\n";
            std::cout << "\nPick a slot to edit:\n\n";
        };

        std::vector<std::string> slotMenu;

        for (size_t i = 0; i < availableSlots.size(); i++)
        {
            auto slotName = getEquipmentSlotName(availableSlots[i]);
            slotMenu.push_back(slotName);
        }
        slotMenu.push_back("Exit");

        auto selection = Utils::pickOptionFromList(prompt, slotMenu);
        if (selection == availableSlots.size())
        {
            return std::make_pair(false, EquipmentSlot::MainHand);
        }
        auto selectedSlot = availableSlots[selection];

        std::cout << "\nSelected slot " << getEquipmentSlotName(selectedSlot) << " (current: " << getEquippedItemName(hero, selectedSlot) << ")\n";

        return std::make_pair(true, selectedSlot);
    }

    void selectPartyEquipment(std::vector<Hero> &heroes)
    {
        std::vector<std::string> names;
        for (auto h : heroes)
        {
            names.push_back(h.name);
        }
        names.push_back("Exit");

        auto selection = Utils::pickOptionFromList([]()
                                                   { Utils::printBorderedText("Manage inventory of:"); },
                                                   names);
        if (selection < names.size() - 1)
        {
            auto &hero = heroes[selection];
            selectEquipment(hero);
        }
    }

    void selectEquipment(Hero &hero)
    {
        while (true)
        {

            Utils::clearScreen();

            auto selectedSlotTuple = selectSlot(hero);
            if (!selectedSlotTuple.first)
            {
                break;
            }
            auto selectedSlot = selectedSlotTuple.second;

            // select the action

            // check if we can equip
            auto listOfEquipable = equipableInHand(hero, selectedSlot);

            bool canUnequip = getEquippedItemName(hero, selectedSlot) != "Empty";
            bool canEquip = listOfEquipable.size() > 0;

            std::vector<std::string> actions;
            if (canUnequip)
            {
                actions.push_back("Unequip");
            }
            if (canEquip)
            {
                actions.push_back("Equip");
            }
            actions.push_back("Back");

            auto heroEquipped = Utils::getEquippedItemsString(hero.inventory.equipped);
            auto heroBackpack = Utils::getBackpack(hero.inventory.backpack);
            auto slotName = getEquipmentSlotName(selectedSlot);

            auto prompt = [hero, heroEquipped, heroBackpack, slotName]()
            {
                Utils::printHeroHeader(hero.name, hero.level);
                std::cout << "|"
                          << "Inventory\n";
                Utils::printBorder(130);
                std::cout << heroEquipped;
                std::cout << heroBackpack;
                Utils::printBorder(130);
                std::cout << "|Manage slot: " << slotName << '\n';
                Utils::printBorder(130);
                std::cout << "\n\n";
                std::cout << "Pick action:\n\n";
            };

            auto selectedAction = actions[Utils::pickOptionFromList(prompt, actions)];

            // execute the action

            if (selectedAction == "Unequip")
            {
                unequipItem(hero, selectedSlot);
            }
            else if (selectedAction == "Equip")
            {
                Utils::clearScreen();

                std::vector<std::string> chooseWeapon;
                for (size_t j = 0; j < listOfEquipable.size(); j++)
                {
                    chooseWeapon.push_back(Utils::getItemString(listOfEquipable[j], true));
                }

                auto pickItemPrompt = []()
                {
                    Utils::printBorderedText("Select equipment:");
                };
                auto itemSelection = Utils::pickOptionFromList(pickItemPrompt, chooseWeapon);
                auto newItem = listOfEquipable[itemSelection];

                equipItem(hero, newItem, selectedSlot);
            }
            else if (selectedAction == "Back")
            {
                continue;
            }
        }
    }

    uint32_t getEquippedArmorValue(const Hero &hero)
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
            auto slotName = getEquipmentSlotName(slot);
            if (hero.inventory.equipped.find(slotName) != hero.inventory.equipped.end())
            {
                auto item = g_AllItems[hero.inventory.equipped.at(slotName)];
                value += item.armor;
            }
        }

        return value;
    }

    uint32_t getEquippedDamageValue(const Hero &hero)
    {

        uint32_t value = 0;

        // handle main hand
        auto mainHandSlot = getEquipmentSlotName(EquipmentSlot::MainHand);
        auto offHandSlot = getEquipmentSlotName(EquipmentSlot::Offhand);

        // check which slots are equipped
        bool isMainHand = hero.inventory.equipped.find(mainHandSlot) != hero.inventory.equipped.end();
        bool isOffHand = hero.inventory.equipped.find(offHandSlot) != hero.inventory.equipped.end();

        // both hands are equipped
        if (isMainHand && isOffHand)
        {
            auto mainItem = g_AllItems[hero.inventory.equipped.at(mainHandSlot)];
            auto offItem = g_AllItems[hero.inventory.equipped.at(offHandSlot)];

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
                value += static_cast<uint32_t>(
                    std::ceil(
                        static_cast<double>(offItem.damage) / 2.0 + 0.25 * static_cast<double>(hero.specialties.dualWielding)));
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
            auto mainItem = g_AllItems[hero.inventory.equipped.at(mainHandSlot)];

            value += mainItem.damage;
            value += getPrimaryAttributeValueFromHero(mainItem, hero);
            value += hero.specialties.oneHanded;
        }
        // only offhand is eqipped
        else if (isOffHand)
        {
            auto offItem = g_AllItems[hero.inventory.equipped.at(offHandSlot)];

            value += offItem.damage;
            value += getPrimaryAttributeValueFromHero(offItem, hero);
            value += hero.specialties.oneHanded;
        }

        // you can still do some damage with empty hands
        if (value == 0)
        {
            value += 5;
        }

        return value;
    }

    const std::string getEquipmentSlotName(const EquipmentSlot eSlot)
    {
        switch (eSlot)
        {
        case EquipmentSlot::MainHand:
            return equipmentSlotNames[0];
        case EquipmentSlot::Offhand:
            return equipmentSlotNames[1];
        case EquipmentSlot::Torso:
            return equipmentSlotNames[2];
        case EquipmentSlot::Head:
            return equipmentSlotNames[3];
        case EquipmentSlot::Legs:
            return equipmentSlotNames[4];
        case EquipmentSlot::Gloves:
            return equipmentSlotNames[5];
        default:
            std::cout << "Bad Slot!\n";
        }

        return equipmentSlotNames[0];
    }

    uint32_t getPrimaryAttributeValueFromHero(const Item &item, const Hero &hero)
    {
        const uint32_t scaleDownBy = 9;

        switch (item.primaryAttribute)
        {
        case PrimaryAttribute::Strength:
            return hero.attributes.strength - scaleDownBy;
        case PrimaryAttribute::Dexterity:
            return hero.attributes.dexterity - scaleDownBy;
        case PrimaryAttribute::Intelligence:
            return hero.attributes.intelligence - scaleDownBy;
        case PrimaryAttribute::None:
            return 0;
        default:
            return 0;
        }
    }
}