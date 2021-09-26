#include "utils.hpp"

#include <iostream>
#include <regex>
#include <iomanip>

std::string getEquipmentSlotName(EquipmentSlot eSlot)
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

void printAttributes(Attributes attributes)
{
    std::cout << "|"
              << "Attributes:\n";
    std::cout << "|" << std::left << std::setw(16) << "    Strength: " << std::left << std::setw(6) << attributes.strength << "\n";
    std::cout << "|" << std::left << std::setw(16) << "    Dexterity: " << std::left << std::setw(6) << attributes.dexterity << "\n";
    std::cout << "|" << std::left << std::setw(16) << "    Vitality: " << std::left << std::setw(6) << attributes.vitality << "\n";
    std::cout << "|" << std::left << std::setw(16) << "    Intelligence: " << std::left << std::setw(6) << attributes.intelligence << "\n";
}

void printHeroAbilities(const std::vector<Ability> &abilities)
{
    std::cout << "|"
              << "Abilities:\n";
    for (auto a : abilities)
    {
        std::cout << "|"
                  << "    " << a.name << " - " << a.description << "\n";
    }
}

void printItem(Item a)
{
    std::cout << std::left << std::setw(15) << a.name << " ";

    std::cout << std::left << std::setw(23);
    switch (a.type)
    {
    case ItemType::Armor_Gloves:
        std::cout << " [Armor gloves] ";
        break;
    case ItemType::Armor_Head:
        std::cout << " [Armor head] ";
        break;
    case ItemType::Armor_Legs:
        std::cout << " [Armor legs] ";
        break;
    case ItemType::Armor_Torso:
        std::cout << " [Armor torso] ";
        break;
    case ItemType::Melee_OneHanded:
        std::cout << " [Melee one-handed] ";
        break;
    case ItemType::Ranged_OneHanded:
        std::cout << " [Ranged one-handed] ";
        break;
    case ItemType::Melee_TwoHanded:
        std::cout << " [Melee two-handed] ";
        break;
    case ItemType::Ranged_TwoHanded:
        std::cout << " [Ranged two-handed] ";
        break;
    case ItemType::Dual_Wielding:
        std::cout << " [Dual wielding] ";
        break;
    case ItemType::Shield:
        std::cout << " [Shield] ";
        break;
    case ItemType::Consumable:
        std::cout << " [Consumable] ";
        break;
    case ItemType::Throwable:
        std::cout << " [Throwable] ";
        break;
    case ItemType::Scroll:
        std::cout << " [Scroll] ";
        break;
    default:
        std::cout << " [Item] ";
        break;
    }

    std::cout << std::left << std::setw(42) << a.description;

    std::cout << "(";
    if (a.damage > 0)
    {
        std::cout << " DMG: " << a.damage;
    }
    if (a.armor > 0)
    {
        std::cout << " DEF: " << a.armor;
    }
    std::cout << " )";
}

void printEquippedItems(const std::unordered_map<std::string, Item> &equipped)
{
    for (auto s : equipmentSlotNames)
    {
        if (equipped.find(s) != equipped.end())
        {
            std::cout << "|";
            std::cout << "    Equipped in ";
            std::cout << std::left << std::setw(10) << s << " - ";
            printItem(equipped.at(s));
            std::cout << "\n";
        }
    }
}

void printBackpack(const std::vector<Item> &backpack)
{
    if (backpack.size() > 0)
    {
        std::cout << "Backpack:\n";
        for (auto a : backpack)
        {
            std::cout << "    ";
            printItem(a);
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

void printHeroInventory(const Inventory &inventory)
{
    std::cout << "|";
    std::cout << "Equippped:\n";
    printEquippedItems(inventory.equipped);
    printBackpack(inventory.backpack);
}

void printHero(Hero hero)
{
    std::cout << "+-------------------------------------------------+\n";
    std::cout << "|" << hero.name << " (Level " << hero.level << ") \n";
    std::cout << "+-------------------------------------------------+\n";
    std::cout << "|"
                 "HP: "
              << hero.health << "/" << hero.maxHealth << "\n";
    std::cout << "|"
                 "XP: "
              << hero.xp << "/" << hero.xpToLevelUp << "\n";
    std::cout << "+-------------------------------------------------+\n";
    printAttributes(hero.attributes);
    if (hero.abilities.size() > 0)
    {
        printHeroAbilities(hero.abilities);
    }
    printHeroInventory(hero.inventory);
    std::cout << "+-------------------------------------------------+\n";
}

unsigned int pickOption(unsigned int numberOfOptions)
{
    unsigned int selection = 0;
    std::cout << "Please choose (1-" << numberOfOptions << "): ";
    std::cin >> selection;

    while (std::cin.fail() || selection < 1 || selection > numberOfOptions)
    {
        std::cout << "Please choose (1-" << numberOfOptions << "): ";
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> selection;
    }

    return selection;
}

unsigned int pickOptionFromList(const std::vector<std::string> &list)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        std::cout << i + 1 << ".) " << list[i] << "\n";
    }
    std::cout << "\n";
    return pickOption(list.size()) - 1;
}

void clearScreen()
{
#if defined _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

std::string trim(std::string s)
{
    // remove leading and trailing spaces
    std::regex e("^\\s+|\\s+$");
    return std::regex_replace(s, e, "");
}

void printIntro()
{
    std::cout << ".s5SSSSs.                         .s5SSSs.                                              .s5SSSs.                                                   \n";
    std::cout << "   SSS    .s    s.  .s5SSSs.            SS. .s5SSSs.  .s5SSSs.  .s5SSSs.  .s5SSSSs.           SS. .s        .s5SSSs.  .s5SSSs.  .s    s.  .s5SSSs. \n";
    std::cout << "   S%S          SS.       SS.     sS    `:;       SS.       SS.       SS.    SSS        sS    S%S                 SS.       SS.       SS.       SS.\n";
    std::cout << "   S%S    sS    S%S sS    `:;     SS        sS    S%S sS    `:; sS    S%S    S%S        SS    S%S sS        sS    S%S sS    `:; sS    S%S sS    `:;\n";
    std::cout << "   S%S    SSSs. S%S SSSs.         SS        SS .sS;:' SSSs.     SSSs. S%S    S%S        SS .sS::' SS        SSSs. S%S SS        SS    S%S SSSs.    \n";
    std::cout << "   S%S    SS    S%S SS            SS        SS    ;,  SS        SS    S%S    S%S        SS        SS        SS    S%S SS        SS    S%S SS       \n";
    std::cout << "   `:;    SS    `:; SS            SS   ``:; SS    `:; SS        SS    `:;    `:;        SS        SS        SS    `:; SS   ``:; SS    `:; SS       \n";
    std::cout << "   ;,.    SS    ;,. SS    ;,.     SS    ;,. SS    ;,. SS    ;,. SS    ;,.    ;,.        SS        SS    ;,. SS    ;,. SS    ;,. SS    ;,. SS    ;,.\n";
    std::cout << "   ;:'    :;    ;:' `:;;;;;:'     `:;;;;;:' `:    ;:' `:;;;;;:' :;    ;:'    ;:'        `:        `:;;;;;:' :;    ;:' `:;;;;;:' `:;;;;;:' `:;;;;;:'\n";
    std::cout << "\n\n";
}

bool askConfirmation(const std::string &question)
{
    std::cout << question << " (Y or n): ";
    std::string confirmation;
    std::cin >> confirmation;
    return confirmation == "Y" || confirmation == "y";
}

std::string enterName()
{
    std::string name = "";

    while (name == "" || trim(name) == "" || name.length() < 1 || std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cout << "Name: ";
        std::cin >> name;
    }

    return trim(name);
}

bool isNameAlreadyInUse(std::string name, const std::vector<Hero> &heroes)
{
    bool sameName = false;
    for (auto h : heroes)
    {
        if (h.name == name)
        {
            sameName = true;
        }
    }
    return sameName;
}