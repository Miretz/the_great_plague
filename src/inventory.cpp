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

        std::cout << "\nPick a starting item:\n\n";

        for (size_t i = 0; i < filtered.size(); i++)
        {
            std::cout << i + 1 << ".) ";
            printItem(filtered[i]);
            std::cout << "\n";
        }
        std::cout << "\n";

        int selection = pickOption(filtered.size());
        return filtered[selection - 1];
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

        std::cout << "\nPick a slot to edit:\n\n";

        for (size_t i = 0; i < availableSlots.size(); i++)
        {
            auto slotName = getEquipmentSlotName(availableSlots[i]);
            std::cout << i + 1 << ".) " << slotName << "\n";
        }

        std::cout << availableSlots.size() + 1 << ".) "
                  << "Exit\n\n";

        auto selection = pickOption(availableSlots.size() + 1);
        if (selection == availableSlots.size() + 1)
        {
            return std::make_pair(false, EquipmentSlot::MainHand);
        }
        auto selectedSlot = availableSlots[selection - 1];

        std::cout << "\nSelected slot " << getEquipmentSlotName(selectedSlot) << " (current: " << getEquippedItemName(hero, selectedSlot) << ")\n";

        return std::make_pair(true, selectedSlot);
    }

    void selectEquipment(Hero &hero)
    {
        while (true)
        {

            clearScreen();

            std::cout << "Managing inventory of " << hero.name << "\n";
            printHeroInventory(hero.inventory);

            // select slot

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

            for (size_t i = 0; i < actions.size(); i++)
            {
                std::cout << i + 1 << ".) " << actions[i] << "\n";
            }
            auto selectedAction = actions[pickOption(actions.size()) - 1];

            // execute the action

            if (selectedAction == "Unequip")
            {
                unequipItem(hero, selectedSlot);
            }
            else if (selectedAction == "Equip")
            {
                clearScreen();

                for (size_t j = 0; j < listOfEquipable.size(); j++)
                {
                    std::cout << j + 1 << ".) ";
                    printItem(listOfEquipable[j]);
                    std::cout << "\n";
                }

                int itemSelection = pickOption(listOfEquipable.size());
                auto newItem = listOfEquipable[itemSelection - 1];

                equipItem(hero, newItem, selectedSlot);
            }
            else if (selectedAction == "Back")
            {
                continue;
            }
        }
    }
}