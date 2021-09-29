#include "utils.hpp"

#include "abilities.hpp"

#include <iostream>
#include <regex>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <conio.h>

#if defined _WIN32
#include <windows.h>
#endif

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

void printHeroAbilities(const std::vector<uint32_t> &abilities)
{
    for (auto id : abilities)
    {
        auto a = Abilities::allAbilities[id];
        std::cout << "|"
                  << "Ability: " << a.name << " - " << a.description << "\n";
    }
}

void printItem(uint32_t a)
{
    std::cout << getItemString(a);
}

std::string getItemString(uint32_t itemId)
{
    auto a = g_AllItems[itemId];
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

std::string getEquippedItemsString(const std::unordered_map<std::string, uint32_t> &equipped)
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

std::string getBackpack(const std::vector<uint32_t> &backpack)
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

void printHeroHeader(const std::string &name, const uint32_t level)
{
    printBorder(130);
    std::cout << "|" << COLOR_GREEN << name << COLOR_END << " (Level " << level << ") \n";
    printBorder(130);
}

void printHero(Hero &hero)
{
    printHeroHeader(hero.name, hero.level);
    std::cout << "|HP: "
              << hero.health << "/" << hero.maxHealth << "\n";
    std::cout << "|XP: "
              << hero.xp << "/" << hero.xpToLevelUp << "\n";
    std::cout << "|Race: " << g_AllRaces.at(hero.race).name << "\n";
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

void printListOfHeroes(const std::vector<Hero> &heroes)
{
    std::cout << "\nList of Heroes:\n";
    for (auto h : heroes)
    {
        std::cout << "\n";
        printHero(h);
    }
    std::cout << "\n";
}

uint32_t slider(std::function<void()> redrawFunction, uint32_t min, uint32_t max)
{
    uint32_t result = min;
    uint32_t c = 0;
    bool finished = false;

    clearScreen();
    redrawFunction();

    std::cout << COLOR_GREY << "\nPress the LEFT & RIGHT arrows to adjust" << COLOR_END << "\n\n";

    uint32_t y = getCursorPosition();

    while (!finished)
    {
        setCursorPosition(0, y);

        std::ostringstream ss;
        ss << "Pick value between " << min << " and " << max << ": ";
        ss << "<-  " << COLOR_YELLOW << std::left << std::setw(2) << result << COLOR_END << " ->\n";

        std::cout << ss.str();

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

uint32_t pickOptionFromList(std::function<void()> redrawFunction, const std::vector<std::string> &list)
{
    uint32_t selected = 0;

    uint32_t c = 0;
    bool finished = false;

    clearScreen();
    redrawFunction();
    uint32_t y = getCursorPosition();

    while (!finished)
    {
        c = 0;

        std::ostringstream ss;
        for (size_t i = 0; i < list.size(); i++)
        {
            if (i == selected)
            {
                ss << COLOR_YELLOW;
                ss << "> ";
            }
            else
            {
                ss << "  ";
            }
            ss << list[i] << "\n\n";
            if (i == selected)
            {
                ss << COLOR_END;
            }
        }

        setCursorPosition(0, y);
        std::cout << ss.str();

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

std::pair<std::vector<uint32_t>, uint32_t> pointsDistributionMenu(std::function<void()> redrawFunction, std::vector<std::pair<std::string, uint32_t>> elements, uint32_t pointsToDistribute)
{
    uint32_t row = 0;

    uint32_t availablePoints = pointsToDistribute;

    // put the base values into the list
    std::vector<uint32_t> values;
    for (auto e : elements)
    {
        values.push_back(e.second);
    }

    uint32_t c = 0;
    bool finished = false;

    clearScreen();
    redrawFunction();

    std::cout << COLOR_GREY << "\nPress UP/DOWN/LEFT/RIGHT to adjust\nPress [Enter] to confirm settings" << COLOR_END << "\n\n";

    uint32_t y = getCursorPosition();

    while (!finished)
    {
        std::ostringstream ss;
        ss << "Available points: ";
        ss << COLOR_GREEN;
        ss << availablePoints;
        ss << COLOR_END;
        ss << "\n\n";

        c = 0;

        for (size_t i = 0; i < elements.size(); i++)
        {
            if (i == row)
            {
                ss << COLOR_YELLOW;
                ss << "> ";
            }
            else
            {
                ss << "  ";
            }
            ss << std::left << std::setw(10) << elements[i].first;
            if (i == row)
            {
                ss << COLOR_END;
            }

            ss << " ";
            ss << "<- " << COLOR_YELLOW << values[i] << COLOR_END << " ->\n\n";
        }

        setCursorPosition(0, y);
        std::cout << ss.str();

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
                break;
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
    std::cout << COLOR_GREEN;
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
    std::cout << COLOR_END;
    std::cout << "---=== Main menu ===---\n\n";
}

bool askConfirmation(const std::string &question)
{
    std::cout << question << " (Y or n): ";

    bool result = false;
    uint32_t c = 0;
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

void pressEnterToContinue()
{
    std::cout << COLOR_GREY << "Press [Enter] to continue..." << COLOR_END << "\n\n";

    fflush(stdin);
    getch();
    fflush(stdin);
}

std::function<void()> createConversationPrompt(std::string who, std::string what, std::string picture)
{
    auto prompt = [who, what, picture]()
    {
        printBorder(130);
        std::cout << "|" << COLOR_GREEN << who << COLOR_END << "\n";
        printBorder(130);
        std::cout << picture;
        printBorder(130);
        std::cout << "|" << what << "\n";
        printBorder(130);
        std::cout << "\n";
    };
    return prompt;
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

void printBorder(uint32_t length)
{
    std::stringstream ss;
    ss << "+";
    for (uint32_t i = 0; i < length - 2; i++)
    {
        ss << "-";
    }
    ss << "+\n";
    std::cout << ss.str();
}

// NOTE: Windows only fix for screen flicker,
// TODO: fix on Unix!

uint32_t getCursorPosition()
{
#if defined _WIN32
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hOut, &csbiInfo);
    return csbiInfo.dwCursorPosition.Y;
#else
    clearScreen();
    std::cout << "Screen redraw not implemented for Unix!";
    exit(-1);
    return 0;
#endif
}

void setCursorPosition(uint32_t x, uint32_t y)
{
#if defined _WIN32
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hOut, coord);
#endif
}

std::string serializeHero(Hero hero)
{
    std::ostringstream ss;

    std::string delimiter = "|";

    ss << hero.name << delimiter;

    ss << hero.health << delimiter;
    ss << hero.xp << delimiter;
    ss << hero.maxHealth << delimiter;
    ss << hero.level << delimiter;
    ss << hero.xpToLevelUp << delimiter;
    ss << hero.unspentPoints << delimiter;

    ss << static_cast<uint32_t>(hero.race) << delimiter;
    ss << static_cast<uint32_t>(hero.controller) << delimiter;

    ss << hero.attributes.strength << delimiter;
    ss << hero.attributes.dexterity << delimiter;
    ss << hero.attributes.vitality << delimiter;
    ss << hero.attributes.intelligence << delimiter;

    for (auto a : hero.abilities)
    {
        ss << a << ";";
    }
    ss << delimiter;

    for (auto b : hero.inventory.backpack)
    {
        ss << b << ";";
    }
    ss << delimiter;

    for (auto a : hero.inventory.equipped)
    {
        ss << a.first << ";" << a.second << ";";
    }
    ss << delimiter;

    return ss.str();
}

Hero deserializeHero(std::string serialized)
{
    std::string delimiter = "|";

    Hero hero;

    hero.name = serialized.substr(0, serialized.find(delimiter));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.health = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.xp = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.maxHealth = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.level = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.xpToLevelUp = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.unspentPoints = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.race = static_cast<Race>(static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter)))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.controller = static_cast<Controller>(static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter)))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.attributes.strength = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.attributes.dexterity = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.attributes.vitality = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    hero.attributes.intelligence = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    auto abilitiesStr = serialized.substr(0, serialized.find(delimiter));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    auto backpackStr = serialized.substr(0, serialized.find(delimiter));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    auto equippedStr = serialized.substr(0, serialized.find(delimiter));
    serialized.erase(0, serialized.find(delimiter) + delimiter.length());

    size_t pos = 0;
    while ((pos = abilitiesStr.find(";")) != std::string::npos)
    {
        auto token = abilitiesStr.substr(0, pos);
        if (token == "")
            break;
        hero.abilities.push_back(static_cast<uint32_t>(std::stoul(token)));
        abilitiesStr.erase(0, pos + delimiter.length());
    }

    pos = 0;
    while ((pos = backpackStr.find(";")) != std::string::npos)
    {
        auto token = backpackStr.substr(0, pos);
        if (token == "")
            break;
        hero.inventory.backpack.push_back(static_cast<uint32_t>(std::stoul(token)));
        backpackStr.erase(0, pos + delimiter.length());
    }

    pos = 0;
    while ((pos = equippedStr.find(";")) != std::string::npos)
    {
        auto key = equippedStr.substr(0, pos);
        if (key == "")
            break;
        equippedStr.erase(0, pos + delimiter.length());
        pos = equippedStr.find(";");
        auto valueStr = equippedStr.substr(0, pos);
        equippedStr.erase(0, pos + delimiter.length());
        if (valueStr == "")
            break;
        auto value = static_cast<uint32_t>(std::stoul(valueStr));
        hero.inventory.equipped[key] = value;
    }

    return hero;
}

void saveGame(std::vector<Hero> &heroes, uint32_t areaId)
{
    std::fstream saveFile;
    saveFile.open(SAVE_FILE, std::ios::out);
    if (saveFile.is_open())
    {
        saveFile << areaId << "\n";
        for (auto h : heroes)
        {
            saveFile << serializeHero(h) << "\n";
        }
        saveFile.close();
    }
}

std::pair<uint32_t, std::vector<Hero>> loadGame()
{
    std::vector<Hero> heroes;
    uint32_t areaId;

    std::fstream saveFile;
    saveFile.open(SAVE_FILE, std::ios::in);
    if (saveFile.is_open())
    {
        std::string line;

        getline(saveFile, line);
        areaId = static_cast<uint32_t>(std::stoul(line));

        while (getline(saveFile, line))
        {
            heroes.push_back(deserializeHero(line));
        }
        saveFile.close();
    }

    return {areaId, heroes};
}