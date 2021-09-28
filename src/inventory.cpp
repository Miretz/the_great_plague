#include "inventory.hpp"

#include "utils.hpp"

#include <iostream>

namespace InventoryManager
{
    Item pickStartingItem(Attributes &attributes)
    {

        std::vector<Item> filtered;
        for (Item it : startingItems)
        {
            auto reqs = it.requirements;
            if (
                attributes.strength >= reqs.strength &&
                attributes.dexterity >= reqs.dexterity &&
                attributes.vitality >= reqs.vitality &&
                attributes.intelligence >= reqs.intelligence)
            {
                filtered.push_back(it);
            }
        }

        clearScreen();

        auto prompt = []()
        {
            printBorder(130);
            std::cout << "|Pick a starting item:\n";
            printBorder(130);
            std::cout << "\n";
        };

        std::vector<std::string> menu;

        for (auto t : filtered)
        {
            menu.push_back(getItemString(t));
        }

        uint32_t selection = pickOptionFromList(prompt, menu);
        return filtered[selection];
    }

    void addToBackpack(Hero &hero, Item item)
    {
        hero.inventory.backpack.push_back(item);
    }

    void removeFromBackpack(Hero &hero, Item item)
    {
        std::vector<Item> newBackpack;
        bool found = false;

        for (auto backpackItem : hero.inventory.backpack)
        {
            if (!found && backpackItem.name == item.name &&
                backpackItem.description == item.description &&
                backpackItem.damage == item.damage &&
                backpackItem.armor == item.armor)
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
            if (hero.inventory.equipped[mainHand].type == ItemType::Melee_TwoHanded ||
                hero.inventory.equipped[mainHand].type == ItemType::Ranged_TwoHanded)
            {
                addToBackpack(hero, hero.inventory.equipped[mainHand]);
                hero.inventory.equipped.erase(mainHand);
                hero.inventory.equipped.erase(offHand);
            }
        }
    }

    void replaceEquipped(Hero &hero, Item item, EquipmentSlot slot)
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
        hero.inventory.equipped[hand] = item;

        if (item.type == ItemType::Melee_TwoHanded || item.type == ItemType::Ranged_TwoHanded)
        {
            auto offHand = getEquipmentSlotName(EquipmentSlot::Offhand);
            if (hero.inventory.equipped.find(offHand) != hero.inventory.equipped.end())
            {
                addToBackpack(hero, hero.inventory.equipped[offHand]);
            }

            hero.inventory.equipped[offHand] = item;
        }

        removeFromBackpack(hero, item);
    }

    void unequipItem(Hero &hero, EquipmentSlot slot)
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

    void equipItem(Hero &hero, Item item, EquipmentSlot slot)
    {
        // Can't equip these types
        if (item.type == ItemType::Scroll || item.type == ItemType::Throwable || item.type == ItemType::Consumable)
        {
            addToBackpack(hero, item);
            return;
        }

        if (hero.attributes.strength >= item.requirements.strength &&
            hero.attributes.dexterity >= item.requirements.dexterity &&
            hero.attributes.vitality >= item.requirements.vitality &&
            hero.attributes.intelligence >= item.requirements.intelligence)
        {
            replaceEquipped(hero, item, slot);
        }
        else
        {
            // can't equip because of attributes - put back to the backpack
            addToBackpack(hero, item);
        }
    }

    std::vector<Item> equipableInHand(const Hero &hero, EquipmentSlot slot)
    {
        std::vector<Item> equipable;

        for (Item item : hero.inventory.backpack)
        {
            if (slot == EquipmentSlot::MainHand)
            {
                if (item.type == ItemType::Melee_OneHanded ||
                    item.type == ItemType::Melee_TwoHanded ||
                    item.type == ItemType::Ranged_OneHanded ||
                    item.type == ItemType::Ranged_TwoHanded ||
                    item.type == ItemType::Dual_Wielding)
                {
                    equipable.push_back(item);
                }
            }
            else if (slot == EquipmentSlot::Offhand)
            {
                if (item.type == ItemType::Shield ||
                    item.type == ItemType::Dual_Wielding)
                {
                    equipable.push_back(item);
                }
            }
            else if (slot == EquipmentSlot::Gloves && item.type == ItemType::Armor_Gloves)
            {
                equipable.push_back(item);
            }
            else if (slot == EquipmentSlot::Torso && item.type == ItemType::Armor_Torso)
            {
                equipable.push_back(item);
            }
            else if (slot == EquipmentSlot::Legs && item.type == ItemType::Armor_Legs)
            {
                equipable.push_back(item);
            }
            else if (slot == EquipmentSlot::Head && item.type == ItemType::Armor_Head)
            {
                equipable.push_back(item);
            }
        }

        return equipable;
    }

    std::string getEquippedItemName(const Hero &hero, EquipmentSlot slot)
    {
        auto slotName = getEquipmentSlotName(slot);
        if (hero.inventory.equipped.find(slotName) != hero.inventory.equipped.end())
        {
            return hero.inventory.equipped.at(slotName).name;
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

        auto heroEquipped = getEquippedItemsString(hero.inventory.equipped);
        auto heroBackpack = getBackpack(hero.inventory.backpack);

        auto prompt = [hero, heroEquipped, heroBackpack]()
        {
            printHeroHeader(hero.name, hero.level);
            std::cout << "|"
                      << "Inventory\n";
            printBorder(130);
            std::cout << heroEquipped;
            std::cout << heroBackpack;
            printBorder(130);
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

        auto selection = pickOptionFromList(prompt, slotMenu);
        if (selection == availableSlots.size())
        {
            return std::make_pair(false, EquipmentSlot::MainHand);
        }
        auto selectedSlot = availableSlots[selection];

        std::cout << "\nSelected slot " << getEquipmentSlotName(selectedSlot) << " (current: " << getEquippedItemName(hero, selectedSlot) << ")\n";

        return std::make_pair(true, selectedSlot);
    }

    void selectEquipment(Hero &hero)
    {
        while (true)
        {

            clearScreen();

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

            auto heroEquipped = getEquippedItemsString(hero.inventory.equipped);
            auto heroBackpack = getBackpack(hero.inventory.backpack);

            auto prompt = [hero, heroEquipped, heroBackpack]()
            {
                printHeroHeader(hero.name, hero.level);
                std::cout << "|"
                          << "Inventory\n";
                printBorder(130);
                std::cout << heroEquipped;
                std::cout << heroBackpack;
                printBorder(130);
                std::cout << "\n\n";
                std::cout << "Pick action:\n\n";
            };

            auto selectedAction = actions[pickOptionFromList(prompt, actions)];

            // execute the action

            if (selectedAction == "Unequip")
            {
                unequipItem(hero, selectedSlot);
            }
            else if (selectedAction == "Equip")
            {
                clearScreen();

                std::vector<std::string> chooseWeapon;
                for (size_t j = 0; j < listOfEquipable.size(); j++)
                {
                    chooseWeapon.push_back(getItemString(listOfEquipable[j]));
                }

                uint32_t itemSelection = pickOptionFromList([]()
                                                       { std::cout << "Select equipment:\n\n"; },
                                                       chooseWeapon);
                auto newItem = listOfEquipable[itemSelection];

                equipItem(hero, newItem, selectedSlot);
            }
            else if (selectedAction == "Back")
            {
                continue;
            }
        }
    }
}