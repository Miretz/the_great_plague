#include "utils.hpp"

#include "abilities.hpp"
#include "inventory_manager.hpp"
#include "files.hpp"

#include <iostream>
#include <regex>
#include <iomanip>
#include <sstream>

#include <conio.h>

#if defined _WIN32
#include <windows.h>
#endif

namespace Utils
{

    void printAttributes(const Attributes &attributes)
    {
        std::cout << "|" << std::left << std::setw(16) << "Strength: " << std::left << std::setw(6) << attributes.strength << '\n';
        std::cout << "|" << std::left << std::setw(16) << "Dexterity: " << std::left << std::setw(6) << attributes.dexterity << '\n';
        std::cout << "|" << std::left << std::setw(16) << "Vitality: " << std::left << std::setw(6) << attributes.vitality << '\n';
        std::cout << "|" << std::left << std::setw(16) << "Intelligence: " << std::left << std::setw(6) << attributes.intelligence << '\n';
    }

    void printAttributesAdjustment(const Attributes &base, const Attributes &adjustment)
    {
        std::cout << "|" << std::left << std::setw(16) << "Strength: " << std::left << std::setw(6) << base.strength << std::left << std::setw(6) << adjustment.strength << '\n';
        std::cout << "|" << std::left << std::setw(16) << "Dexterity: " << std::left << std::setw(6) << base.dexterity << std::left << std::setw(6) << adjustment.dexterity << '\n';
        std::cout << "|" << std::left << std::setw(16) << "Vitality: " << std::left << std::setw(6) << base.vitality << std::left << std::setw(6) << adjustment.vitality << '\n';
        std::cout << "|" << std::left << std::setw(16) << "Intelligence: " << std::left << std::setw(6) << base.intelligence << std::left << std::setw(6) << adjustment.intelligence << '\n';
    }

    void printHeroAbilities(const std::vector<uint32_t> &abilities)
    {
        for (auto id : abilities)
        {
            auto a = Abilities::allAbilities[id];
            std::cout << "|"
                      << "Ability: " << a.name << " - " << a.description << '\n';
        }
    }

    void printItem(const uint32_t a)
    {
        std::cout << getItemString(a);
    }

    const std::string getItemString(const uint32_t itemId)
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

    const std::string getEquippedItemsString(const std::unordered_map<std::string, uint32_t> &equipped)
    {
        std::stringstream ss;
        for (auto s : InventoryManager::equipmentSlotNames)
        {
            if (equipped.find(s) != equipped.end())
            {
                ss << '|';
                ss << "Equipped in ";
                ss << std::left << std::setw(10) << s << " - ";
                ss << getItemString(equipped.at(s));
                ss << '\n';
            }
        }
        return ss.str();
    }

    const std::string getBackpack(const std::vector<uint32_t> &backpack)
    {
        std::stringstream ss;
        if (backpack.size() > 0)
        {
            ss << '|'
               << "Backpack:\n";
            for (auto a : backpack)
            {
                ss << '|';
                ss << "    ";
                ss << getItemString(a);
                ss << '\n';
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
        std::cout << '|' << COLOR_GREEN << name << COLOR_END << " (Level " << level << ") \n";
        printBorder(130);
    }

    void printCombatHeroHeader(const Hero &hero)
    {
        printBorder(130);

        std::ostringstream ss;

        ss << '|';
        const auto color = hero.controller == Controller::AI_Enemy ? COLOR_RED : COLOR_GREEN;

        // hero name
        ss << color;
        ss << std::left << std::setw(20);
        ss << hero.name;
        ss << COLOR_END;
        ss << " (Level " << hero.level << ") ";

        ss << std::left << std::setw(10);
        ss << " | HP: ";
        ss << std::left << std::setw(10);
        ss << std::to_string(hero.health) + "/" + std::to_string(hero.maxHealth);
        ss << '|';

        // health bar
        auto healthBarColor = hero.health < 50 ? COLOR_YELLOW : COLOR_GREEN;
        healthBarColor = hero.health < 25 ? COLOR_RED : healthBarColor;
        ss << healthBarColor;
        ss << std::left << std::setw(20);

        std::ostringstream healthBarSs;
        for (uint32_t i = 0; i < hero.health; i += 10)
        {
            healthBarSs << '|';
        }
        ss << healthBarSs.str();

        ss << COLOR_END;

        // armor
        ss << std::left << std::setw(20);
        ss << " | Armor: ";
        ss << std::left << std::setw(10);

        // race
        ss << InventoryManager::getEquippedArmorValue(hero);
        ss << " | Race: " << g_AllRaces.at(hero.race).name << '\n';

        std::cout << ss.str();

        printBorder(130);
    }

    void printBorderedText(const std::string &text)
    {
        printBorder(130);
        std::cout << "|" << text << '\n';
        printBorder(130);
    }

    void printSpacedText(const std::string &text)
    {
        std::cout << "\n"
                  << text << "\n\n";
    }

    void newLine()
    {
        std::cout << '\n';
    }

    void printHero(const Hero &hero)
    {
        printHeroHeader(hero.name, hero.level);
        std::cout << "|HP: "
                  << hero.health << "/" << hero.maxHealth;
        std::cout << " Armor: " << InventoryManager::getEquippedArmorValue(hero) << '\n';
        std::cout << "|XP: "
                  << hero.xp << "/" << hero.xpToLevelUp << '\n';
        std::cout << "|Race: " << g_AllRaces.at(hero.race).name << '\n';
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
            std::cout << '\n';
            printHero(h);
        }
        std::cout << '\n';
    }

    uint32_t slider(std::function<void()> redrawFunction, const uint32_t min, const uint32_t max)
    {
        uint32_t result = min;
        uint32_t c = 0;
        bool finished = false;

        clearScreen();
        redrawFunction();
        newLine();

        std::cout << COLOR_GREY << "Press the LEFT & RIGHT arrows to adjust" << COLOR_END << "\n\n";

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
        newLine();

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
            ss << "      ";
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
        std::cout << Files::loadFile("assets/logo.txt");
        std::cout << COLOR_END;
        std::cout << "---=== Main menu ===---\n";
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

    std::function<void()> createConversationPrompt(const std::string &who, const std::string &what, const std::string &picture)
    {
        auto prompt = [who, what, picture]()
        {
            printBorder(130);
            std::cout << "|" << COLOR_GREEN << who << COLOR_END << '\n';
            printBorder(130);
            std::cout << picture;
            printBorder(130);
            std::cout << "|" << what << '\n';
            printBorder(130);
        };
        return prompt;
    }

    const std::string enterName()
    {
        newLine();
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

    void printBorder(const uint32_t length)
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

    void setCursorPosition(const uint32_t x, const uint32_t y)
    {
#if defined _WIN32
        static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        std::cout.flush();
        COORD coord = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(hOut, coord);
#endif
    }

}