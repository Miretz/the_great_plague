#include "utils.hpp"

#include <iostream>
#include <regex>
#include <iomanip>
#include <sstream>

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
    std::cout << "|" << std::left << std::setw(16) << "Strength: " << std::left << std::setw(6) << attributes.strength << "\n";
    std::cout << "|" << std::left << std::setw(16) << "Dexterity: " << std::left << std::setw(6) << attributes.dexterity << "\n";
    std::cout << "|" << std::left << std::setw(16) << "Vitality: " << std::left << std::setw(6) << attributes.vitality << "\n";
    std::cout << "|" << std::left << std::setw(16) << "Intelligence: " << std::left << std::setw(6) << attributes.intelligence << "\n";
}

void printAttributesAdjustment(Attributes base, Attributes adjustment)
{
    std::cout << "|" << std::left << std::setw(16) << "Strength: " << std::left << std::setw(6) << base.strength << std::left << std::setw(6) << adjustment.strength << "\n";
    std::cout << "|" << std::left << std::setw(16) << "Dexterity: " << std::left << std::setw(6) << base.dexterity << std::left << std::setw(6) << adjustment.dexterity << "\n";
    std::cout << "|" << std::left << std::setw(16) << "Vitality: " << std::left << std::setw(6) << base.vitality << std::left << std::setw(6) << adjustment.vitality << "\n";
    std::cout << "|" << std::left << std::setw(16) << "Intelligence: " << std::left << std::setw(6) << base.intelligence << std::left << std::setw(6) << adjustment.intelligence << "\n";
}

void printHeroAbilities(const std::vector<Ability> &abilities)
{
    for (auto a : abilities)
    {
        std::cout << "|"
                  << "Ability: " << a.name << " - " << a.description << "\n";
    }
}

void printItem(Item a)
{
    std::cout << getItemString(a);
}

std::string getItemString(Item a)
{
    std::stringstream ss;
    ss << std::left << std::setw(15) << a.name << " ";

    ss << std::left << std::setw(23);
    switch (a.type)
    {
    case ItemType::Armor_Gloves:
        ss << " [Armor gloves] ";
        break;
    case ItemType::Armor_Head:
        ss << " [Armor head] ";
        break;
    case ItemType::Armor_Legs:
        ss << " [Armor legs] ";
        break;
    case ItemType::Armor_Torso:
        ss << " [Armor torso] ";
        break;
    case ItemType::Melee_OneHanded:
        ss << " [Melee one-handed] ";
        break;
    case ItemType::Ranged_OneHanded:
        ss << " [Ranged one-handed] ";
        break;
    case ItemType::Melee_TwoHanded:
        ss << " [Melee two-handed] ";
        break;
    case ItemType::Ranged_TwoHanded:
        ss << " [Ranged two-handed] ";
        break;
    case ItemType::Dual_Wielding:
        ss << " [Dual wielding] ";
        break;
    case ItemType::Shield:
        ss << " [Shield] ";
        break;
    case ItemType::Consumable:
        ss << " [Consumable] ";
        break;
    case ItemType::Throwable:
        ss << " [Throwable] ";
        break;
    case ItemType::Scroll:
        ss << " [Scroll] ";
        break;
    default:
        ss << " [Item] ";
        break;
    }

    ss << std::left << std::setw(42) << a.description;

    ss << "(";
    if (a.damage > 0)
    {
        ss << " DMG: " << a.damage;
    }
    if (a.armor > 0)
    {
        ss << " DEF: " << a.armor;
    }
    ss << " )";
    return ss.str();
}

std::string getEquippedItemsString(const std::unordered_map<std::string, Item> &equipped)
{
    std::stringstream ss;
    for (auto s : equipmentSlotNames)
    {
        if (equipped.find(s) != equipped.end())
        {
            ss << "|";
            ss << "Equipped in ";
            ss << std::left << std::setw(10) << s << " - ";
            ss << getItemString(equipped.at(s));
            ss << "\n";
        }
    }
    return ss.str();
}

std::string getBackpack(const std::vector<Item> &backpack)
{
    std::stringstream ss;
    if (backpack.size() > 0)
    {
        ss << "|"
           << "Backpack:\n";
        for (auto a : backpack)
        {
            ss << "|";
            ss << "    ";
            ss << getItemString(a);
            ss << "\n";
        }
    }
    return ss.str();
}

void printHeroInventory(const Inventory &inventory)
{
    std::cout << getEquippedItemsString(inventory.equipped);
    std::cout << getBackpack(inventory.backpack);
}

void printHeroHeader(const std::string &name, const int level)
{
    printBorder(130);
    std::cout << "|" << COLOR_GREEN << name << COLOR_END << " (Level " << level << ") \n";
    printBorder(130);
}

void printHero(Hero &hero)
{
    printHeroHeader(hero.name, hero.level);
    std::cout << "|"
                 "HP: "
              << hero.health << "/" << hero.maxHealth << "\n";
    std::cout << "|"
                 "XP: "
              << hero.xp << "/" << hero.xpToLevelUp << "\n";
    printBorder(130);
    printAttributes(hero.attributes);
    printBorder(130);
    if (hero.abilities.size() > 0)
    {
        printHeroAbilities(hero.abilities);
    }
    printBorder(130);
    printHeroInventory(hero.inventory);
    printBorder(130);
}

int slider(std::function<void()> redrawFunction, int min, int max)
{
    int result = min;
    int c = 0;
    bool finished = false;

    while (!finished)
    {
        clearScreen();
        redrawFunction();

        std::cout << "Pick value between " << min << " and " << max << ": ";
        std::cout << "<- " << COLOR_YELLOW << result << COLOR_END << " ->\n";

        fflush(stdin);
        switch ((c = getch()))
        {
        case KEY_LEFT:
            result--;
            if (result < min)
            {
                result = min;
            }
            break;
        case KEY_RIGHT:
            result++;
            if (result > max)
            {
                result = max;
            }
            break;
        case KEY_ENTER:
            finished = true;
            break;
        case KEY_ENTER_LF:
            finished = true;
            break;
        default:
            break;
        }
    }

    fflush(stdin);

    return result;
}

unsigned int pickOptionFromList(std::function<void()> redrawFunction, const std::vector<std::string> &list)
{
    unsigned int selected = 0;

    int c = 0;
    bool finished = false;

    while (!finished)
    {
        clearScreen();

        redrawFunction();

        c = 0;

        for (size_t i = 0; i < list.size(); i++)
        {
            if (i == selected)
            {
                std::cout << COLOR_YELLOW;
                std::cout << "> ";
            }
            else
            {
                std::cout << "  ";
            }
            std::cout << list[i] << "\n";
            if (i == selected)
            {
                std::cout << COLOR_END;
            }
        }

        fflush(stdin);
        switch ((c = getch()))
        {
        case KEY_UP:
            if (selected == 0)
            {
                selected = list.size();
            }
            selected--;

            break;
        case KEY_DOWN:
            selected++;
            if (selected > list.size() - 1)
            {
                selected = 0;
            }
            break;
        case KEY_ENTER:
            finished = true;
            break;
        case KEY_ENTER_LF:
            finished = true;
            break;
        default:
            break;
        }
    }

    fflush(stdin);

    return selected;
}

std::pair<std::vector<int>, int> pointsDistributionMenu(std::function<void()> redrawFunction, std::vector<std::pair<std::string, int>> elements, int pointsToDistribute)
{
    unsigned int row = 0;

    int availablePoints = pointsToDistribute;

    // put the base values into the list
    std::vector<int> values;
    for (auto e : elements)
    {
        values.push_back(e.second);
    }

    int c = 0;
    bool finished = false;

    while (!finished)
    {
        clearScreen();

        redrawFunction();

        std::cout << "Available points: ";
        std::cout << COLOR_GREEN;
        std::cout << availablePoints;
        std::cout << COLOR_END;
        std::cout << "\n\n";

        c = 0;

        for (size_t i = 0; i < elements.size(); i++)
        {
            if (i == row)
            {
                std::cout << COLOR_YELLOW;
                std::cout << "> ";
            }
            else
            {
                std::cout << "  ";
            }
            std::cout << std::left << std::setw(10) << elements[i].first;
            if (i == row)
            {
                std::cout << COLOR_END;
            }

            std::cout << " ";
            std::cout << "<- " << COLOR_YELLOW << values[i] << COLOR_END << " ->\n";
        }

        fflush(stdin);
        switch ((c = getch()))
        {
        case KEY_UP:
            if (row == 0)
            {
                row = elements.size();
            }
            row--;
            break;
        case KEY_DOWN:
            row++;
            if (row > elements.size() - 1)
            {
                row = 0;
            }
            break;
        case KEY_LEFT:
            if (availablePoints == pointsToDistribute)
            {
                break;
            }
            if (values[row] <= elements[row].second)
            {
                values[row] = elements[row].second;
            }
            values[row] -= 1;
            availablePoints++;
            break;
        case KEY_RIGHT:
            if (availablePoints == 0)
            {
                break;
            }
            values[row] += 1;
            availablePoints--;
            break;
        case KEY_ENTER:
            finished = true;
            break;
        case KEY_ENTER_LF:
            finished = true;
            break;
        default:
            break;
        }
    }

    fflush(stdin);

    return {values, availablePoints};
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
    std::cout << "---=== Main menu ===---\n";
}

bool askConfirmation(const std::string &question)
{
    std::cout << question << " (Y or n): ";

    bool result = false;
    int c = 0;
    fflush(stdin);
    switch ((c = getch()))
    {
    case 89: // y
        result = true;
        break;
    case 121: // Y
        result = true;
        break;
    case KEY_ENTER_LF: // Line Feed
        result = true;
        break;
    case KEY_ENTER:
        result = true;
        break;
    default:
        break;
    }

    return result;
}

std::string enterName()
{
    std::string name = "";
    std::cout << "Name: ";
    std::cin >> name;

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

void printBorder(int length)
{
    std::cout << "+";
    for (int i = 0; i < length - 2; i++)
    {
        std::cout << "-";
    }
    std::cout << "+\n";
}