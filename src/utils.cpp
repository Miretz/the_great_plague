#include "utils.hpp"

#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

#include "abilities.hpp"
#include "files.hpp"
#include "inventory_manager.hpp"

#if defined _WIN32
#include <conio.h>
#include <windows.h>

#include <cwchar>

#else

#include <termios.h>
#include <unistd.h>

#endif

namespace Utils
{
    void maximizeConsole()
    {
#if defined _WIN32
        // Windowed fullscreen
        /*
        COORD ScreenBufferSize{ 1500, 300 };
        HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        HWND ConsoleWindow = GetConsoleWindow();

        SetConsoleScreenBufferSize(ConsoleHandle, ScreenBufferSize);
        ShowWindow(ConsoleWindow, SW_MAXIMIZE);
        */

        // Fullscreen without header
        SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);  // NOLINT

        // Change font
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;   // Width of each character in the font
        cfi.dwFontSize.Y = 20;  // NOLINT
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        std::wcscpy(cfi.FaceName, L"Consolas");  // NOLINT
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
#endif
    }

    auto addPlus(uint32_t value) -> const std::string
    {
        if (value > 0)
        {
            return "+" + std::to_string(value);
        }
        return " 0";
    }

    void printAttributes(const Attributes &attributes)
    {
        std::cout << '|' << std::left << std::setw(kPaddingLeft) << "Strength: " << std::left << std::setw(kPaddingRight)
                  << attributes.strength << '\n';
        std::cout << '|' << std::left << std::setw(kPaddingLeft) << "Dexterity: " << std::left << std::setw(kPaddingRight)
                  << attributes.dexterity << '\n';
        std::cout << '|' << std::left << std::setw(kPaddingLeft) << "Constitution: " << std::left << std::setw(kPaddingRight)
                  << attributes.constitution << '\n';
        std::cout << '|' << std::left << std::setw(kPaddingLeft) << "Intelligence: " << std::left << std::setw(kPaddingRight)
                  << attributes.intelligence << '\n';
    }

    void printAttributesAdjustment(const Attributes &base, const Attributes &adjustment)
    {
        std::cout << '|' << std::left << std::setw(kPaddingLeft) << "Strength: " << std::left << std::setw(kPaddingRight)
                  << base.strength << std::left << std::setw(kPaddingRight) << adjustment.strength << '\n';
        std::cout << '|' << std::left << std::setw(kPaddingLeft) << "Dexterity: " << std::left << std::setw(kPaddingRight)
                  << base.dexterity << std::left << std::setw(kPaddingRight) << adjustment.dexterity << '\n';
        std::cout << '|' << std::left << std::setw(kPaddingLeft) << "Constitution: " << std::left << std::setw(kPaddingRight)
                  << base.constitution << std::left << std::setw(kPaddingRight) << adjustment.constitution << '\n';
        std::cout << '|' << std::left << std::setw(kPaddingLeft) << "Intelligence: " << std::left << std::setw(kPaddingRight)
                  << base.intelligence << std::left << std::setw(kPaddingRight) << adjustment.intelligence << '\n';
    }

    void printSpecialties(const Specialties &specialties)
    {
        auto printSpecialty = [](const std::string &name, uint32_t value)
        {
            if (value > 0)
            {
                std::cout << '|' << std::left << std::setw(kPaddingLeft) << name << std::left << std::setw(kPaddingRight)
                          << addPlus(value) << '\n';
            }
        };

        printSpecialty("One-handed", specialties.oneHanded);
        printSpecialty("Two-handed", specialties.twoHanded);
        printSpecialty("Ranged", specialties.ranged);
        printSpecialty("Dual wielding", specialties.dualWielding);

        printSpecialty("Mysticism", specialties.mysticism);
        printSpecialty("Terramancy", specialties.terramancy);
        printSpecialty("Hydromancy", specialties.hydromancy);
        printSpecialty("Pyromancy", specialties.pyromancy);
        printSpecialty("Aeromancy", specialties.aeromancy);
    }

    void printSpecialtiesAdjustment(const Specialties &base, const Specialties &adjustment)
    {
        auto printAdjustment = [](const std::string &name, uint32_t v1, uint32_t v2)
        {
            if (v2 > v1)
            {
                std::cout << '|' << std::left << std::setw(kPaddingLeft) << name << std::left << std::setw(kPaddingRight)
                          << addPlus(v1) << std::left << std::setw(kPaddingRight) << addPlus(v2) << '\n';
            }
        };

        printAdjustment("One-handed", base.oneHanded, adjustment.oneHanded);
        printAdjustment("Two-handed", base.twoHanded, adjustment.twoHanded);
        printAdjustment("Ranged", base.ranged, adjustment.ranged);
        printAdjustment("Dual wielding", base.dualWielding, adjustment.dualWielding);

        printAdjustment("Mysticism", base.mysticism, adjustment.mysticism);
        printAdjustment("Terramancy", base.terramancy, adjustment.terramancy);
        printAdjustment("Hydromancy", base.hydromancy, adjustment.hydromancy);
        printAdjustment("Pyromancy", base.pyromancy, adjustment.pyromancy);
        printAdjustment("Aeromancy", base.aeromancy, adjustment.aeromancy);
    }

    void printHeroAbilities(const std::vector<std::string> &abilities)
    {
        for (const auto &id : abilities)
        {
            const auto a = Abilities::getAbility(id).value();
            std::cout << "|Ability: " << a.name << " - " << a.description << '\n';
        }
    }

    auto getItemString(const uint32_t itemId, bool describe) -> const std::string
    {
        auto a = g_AllItems[itemId];
        std::stringstream ss;
        ss << std::left << std::setw(40) << a.name << ' ';  // NOLINT

        ss << std::left << std::setw(20);  // NOLINT

        switch (a.primaryAttribute)
        {
            case PrimaryAttribute::Strength: ss << " [Strength] "; break;
            case PrimaryAttribute::Dexterity: ss << " [Dexterity] "; break;
            case PrimaryAttribute::Intelligence: ss << " [Intelligence] "; break;
            default: ss << " [None] "; break;
        }

        ss << std::left << std::setw(23);  // NOLINT
        switch (a.type)
        {
            case ItemType::Armor_Gloves: ss << " [Armor gloves] "; break;
            case ItemType::Armor_Head: ss << " [Armor head] "; break;
            case ItemType::Armor_Legs: ss << " [Armor legs] "; break;
            case ItemType::Armor_Torso: ss << " [Armor torso] "; break;
            case ItemType::Melee_OneHanded: ss << " [Melee one-handed] "; break;
            case ItemType::Ranged_OneHanded: ss << " [Ranged one-handed] "; break;
            case ItemType::Melee_TwoHanded: ss << " [Melee two-handed] "; break;
            case ItemType::Ranged_TwoHanded: ss << " [Ranged two-handed] "; break;
            case ItemType::Shield: ss << " [Shield] "; break;
            case ItemType::Consumable: ss << " [Consumable] "; break;
            case ItemType::Throwable: ss << " [Throwable] "; break;
            case ItemType::Scroll: ss << " [Scroll] "; break;
            default: ss << " [Item] "; break;
        }

        ss << '(';
        if (a.damage > 0)
        {
            ss << " Base DMG: " << a.damage;
        }
        if (a.armor > 0)
        {
            ss << " Defence: " << a.armor;
        }
        ss << " )";

        if (describe)
        {
            ss << "\n  " << COLOR_END << COLOR_GREY << a.description << COLOR_END;
        }

        return ss.str();
    }

    auto getAbilityString(const Ability &ability) -> const std::string
    {
        std::stringstream ss;
        ss << ability.name;
        ss << "\n  " << COLOR_END << COLOR_GREY << ability.description << COLOR_END;
        return ss.str();
    }

    auto getEquippedItemsString(const std::unordered_map<std::string, uint32_t> &equipped) -> const std::string
    {
        std::stringstream ss;
        for (const auto &s : InventoryManager::equipmentSlotNames)
        {
            if (equipped.find(s) != equipped.end())
            {
                ss << '|';
                ss << "Equipped in ";
                ss << std::left << std::setw(10) << s << " - ";  // NOLINT
                ss << getItemString(equipped.at(s), false);
                ss << '\n';
            }
        }
        return ss.str();
    }

    auto getBackpack(const std::vector<uint32_t> &backpack) -> const std::string
    {
        std::stringstream ss;
        if (backpack.size() > 0)
        {
            ss << '|' << "Backpack:\n";
            for (auto a : backpack)
            {
                ss << '|';
                ss << "    ";
                ss << getItemString(a, false);
                ss << '\n';
            }
        }
        return ss.str();
    }

    void printHeroInventory(const Inventory &inventory)
    {
        printBorder();
        std::cout << getEquippedItemsString(inventory.equipped);
        std::cout << getBackpack(inventory.backpack);
        printBorder();
    }

    void printHeroHeader(const std::string &name, const uint32_t level)
    {
        printBorder();
        std::cout << '|' << COLOR_GREEN << name << COLOR_END << " (Level " << level << ") \n";
        printBorder();
    }

    void printCombatHeroHeader(const Hero &hero)
    {
        printBorder();

        std::ostringstream ss;

        ss << '|';
        const auto color = hero.controller == Controller::AI_Enemy ? COLOR_RED : COLOR_GREEN;

        // hero name
        ss << color;
        ss << std::left << std::setw(10);  // NOLINT
        ss << hero.name;
        ss << COLOR_END;
        ss << " (Level " << hero.level << ") ";

        ss << " | AP: " << COLOR_YELLOW << std::to_string(hero.actionPoints) << COLOR_END;
        ss << " | HP: ";
        ss << std::left << std::setw(8);  // NOLINT
        ss << std::to_string(hero.health);
        ss << '/';
        ss << std::to_string(hero.maxHealth);
        ss << '|';

        // health bar
        auto healthBarColor = hero.health < 50 ? COLOR_YELLOW : COLOR_GREEN;  // NOLINT
        healthBarColor = hero.health < 25 ? COLOR_RED : healthBarColor;       // NOLINT
        ss << healthBarColor;
        ss << std::left << std::setw(20);  // NOLINT

        std::ostringstream healthBarSs;
        for (uint32_t i = 0; i < hero.health; i += 10)  // NOLINT
        {
            healthBarSs << '|';
        }
        ss << healthBarSs.str();

        ss << COLOR_END;

        // armor
        ss << " | Armor: ";
        ss << InventoryManager::getEquippedArmorValue(hero);
        ss << " | Race: " << g_AllRaces.at(hero.race).name << '\n';

        std::cout << ss.str();

        // print status effects
        if (hero.statusEffects.size() > 0)
        {
            std::ostringstream ssfx;

            ssfx << "|Effects: ~ ";
            for (const auto &se : hero.statusEffects)
            {
                ssfx << COLOR_YELLOW << se.name << COLOR_END << " (" << se.turnsLeft << " turns) ~ ";
            }
            ssfx << '\n';

            std::cout << ssfx.str();
        }

        printBorder();
    }

    void printBorderedText(const std::string &text)
    {
        printBorder();
        std::cout << '|' << text << '\n';
        printBorder();
    }

    void printBorderedTextWithColor(const std::string &text, const std::string &color)
    {
        printBorder();
        std::cout << '|';
        std::cout << color;
        std::cout << text << '\n';
        std::cout << COLOR_END;
        printBorder();
    }

    auto colorize(const std::string &text, const std::string &color) -> const std::string
    {
        std::ostringstream ss;
        ss << color;
        ss << text;
        ss << COLOR_END;
        return ss.str();
    }

    void printSpacedText(const std::string &text)
    {
        std::cout << '\n' << text << "\n\n";
    }

    void printArea(const std::string &image, const std::string &description)
    {
        std::cout << Utils::COLOR_GREEN;
        std::cout << image;
        std::cout << Utils::COLOR_END;
        std::cout << description;
    }

    void printCombatStart(const std::string &description)
    {
        printBorder();
        std::cout << '|' << Utils::COLOR_RED << "You are under attack!" << Utils::COLOR_END << '\n';
        printBorder();
        std::cout << Utils::COLOR_RED;
        std::cout << Files::loadFile("assets/other/combat_image.txt");
        std::cout << Utils::COLOR_END;
        std::cout << description;
    }

    void newLine()
    {
        std::cout << '\n';
    }

    auto getFullPhysicalDamage(const Hero &hero) -> const std::string
    {
        std::ostringstream ss;
        ss << "|Full physical Damage: " << InventoryManager::getEquippedDamageValue(hero);
        ss << COLOR_GREY << " (Based on equipped weapons + Attributes + Specialties) " << COLOR_END << '\n';
        return ss.str();
    }

    void printFullPhysicalDamage(const Hero &hero)
    {
        std::cout << getFullPhysicalDamage(hero);
    }

    void printHero(const Hero &hero)
    {
        printHeroHeader(hero.name, hero.level);
        std::cout << "|HP: " << hero.health << "/" << hero.maxHealth;
        std::cout << " Armor: " << InventoryManager::getEquippedArmorValue(hero) << '\n';
        std::cout << "|XP: " << hero.xp << "/" << hero.xpToLevelUp << '\n';
        std::cout << "|Race: " << g_AllRaces.at(hero.race).name << '\n';
        printBorder();
        printAttributes(hero.attributes);
        printBorder();
        printSpecialties(hero.specialties);
        printBorder();
        if (hero.abilities.size() > 0)
        {
            printHeroAbilities(hero.abilities);
        }
        printBorder();
        printFullPhysicalDamage(hero);
        printHeroInventory(hero.inventory);
    }

    void printListOfHeroes(const std::vector<Hero> &heroes)
    {
        std::cout << "\nList of Heroes:\n";
        for (const auto &h : heroes)
        {
            std::cout << '\n';
            printHero(h);
        }
        std::cout << '\n';
    }

    auto getInput() -> uint32_t
    {
        uint32_t result = 0;

        std::cin.clear();

#if defined _WIN32
        result = _getch();
#else
        std::cout << '\n';

        char buf = 0;
        termios old{};
        if (tcgetattr(0, &old) < 0)
        {
            std::cout << "Error: tcgetattr error\n";
            exit(1);
        }
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
        {
            std::cout << "Error: tcsetattr error\n";
            exit(1);
        }

        if (read(0, &buf, 1) < 0)
        {
            std::cout << "Error: termios read()\n";
            exit(1);
        }
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
        {
            std::cout << "Error: tcsetattr ~ICANON\n";
            exit(1);
        }
        result = static_cast<unsigned char>(buf);
#endif
        return result;
    }

    auto slider(const std::function<void()> &redrawFunction, const uint32_t min, const uint32_t max) -> uint32_t
    {
        uint32_t result = min;

        clearScreen();
        redrawFunction();
        newLine();

        std::cout << COLOR_GREY << "Press the LEFT & RIGHT arrows to adjust" << COLOR_END << "\n\n";

        uint32_t y = getCursorPosition();

        bool finished = false;
        while (!finished)
        {
            setCursorPosition(0, y);

            std::ostringstream ss;
            ss << "Pick value between " << min << " and " << max << ": ";
            ss << "<-  " << COLOR_YELLOW << std::left << std::setw(2) << result << COLOR_END << " ->\n";

            std::cout << ss.str();

            uint32_t c = 0;
            switch ((c = getInput()))
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
                case KEY_ENTER_LF: finished = true; break;
                default: break;
            }
        }

        return result;
    }

    auto pickOptionFromList(const std::function<void()> &redrawFunction, const std::vector<std::string> &list) -> uint32_t
    {
        uint32_t selected = 0;

        clearScreen();
        redrawFunction();
        newLine();

        uint32_t y = getCursorPosition();

        bool finished = false;

        while (!finished)
        {
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

            uint32_t c = 0;
            switch ((c = getInput()))
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
                case KEY_ENTER_LF: finished = true; break;
                default: break;
            }
        }

        return selected;
    }

    auto pointsDistributionMenu(
        const std::function<void()> &redrawFunction,
        PointsMenuInput elements,
        uint32_t pointsToDistribute) -> std::pair<std::vector<uint32_t>, uint32_t>
    {
        uint32_t row = 0;

        uint32_t availablePoints = pointsToDistribute;

        // put the base values into the list
        std::vector<uint32_t> values;
        std::transform(
            elements.begin(),
            elements.end(),
            std::back_inserter(values),
            [](const auto &e) -> uint32_t
            {
                auto [name, desc, value] = e;
                return value;
            });

        clearScreen();
        redrawFunction();

        std::cout << COLOR_GREY << "\nPress UP/DOWN/LEFT/RIGHT to adjust\nPress [Enter] to confirm settings" << COLOR_END
                  << "\n\n";

        uint32_t y = getCursorPosition();

        bool finished = false;
        while (!finished)
        {
            std::ostringstream ss;
            ss << "Available points: ";
            ss << COLOR_GREEN;
            ss << availablePoints;
            ss << "      ";
            ss << COLOR_END;
            ss << "\n\n";

            for (size_t i = 0; i < elements.size(); i++)
            {
                auto [name, description, value] = elements[i];

                if (i == row)
                {
                    ss << COLOR_YELLOW;
                    ss << "> ";
                }
                else
                {
                    ss << "  ";
                }
                ss << std::left << std::setw(15) << name;  // NOLINT
                if (i == row)
                {
                    ss << COLOR_END;
                }

                ss << " <- " << COLOR_YELLOW << values[i] << COLOR_END << " ->\n  ";
                ss << COLOR_GREY;
                ss << description;
                ss << COLOR_END;
                ss << "\n\n";
            }

            setCursorPosition(0, y);
            std::cout << ss.str();

            uint32_t c = 0;
            switch ((c = getInput()))
            {
                case KEY_UP:
                {
                    if (row == 0)
                    {
                        row = elements.size();
                    }
                    row--;
                    break;
                }
                case KEY_DOWN:
                {
                    row++;
                    if (row > elements.size() - 1)
                    {
                        row = 0;
                    }
                    break;
                }
                case KEY_LEFT:
                {
                    if (availablePoints == pointsToDistribute)
                    {
                        break;
                    }
                    const auto [name, desc, value] = elements[row];
                    if (values[row] <= value)
                    {
                        values[row] = value;
                        break;
                    }
                    values[row] -= 1;
                    availablePoints++;
                    break;
                }
                case KEY_RIGHT:
                {
                    if (availablePoints == 0)
                    {
                        break;
                    }
                    values[row] += 1;
                    availablePoints--;
                    break;
                }
                case KEY_ENTER:
                case KEY_ENTER_LF: finished = true; break;
                default: break;
            }
        }

        return { values, availablePoints };
    }

    void clearScreen()
    {
#if defined _WIN32
        system("cls");
#else
        std::cout << "\033[2J\033[1;1H";
#endif
    }

    auto trim(const std::string &s) -> std::string
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

    auto askConfirmation(const std::string &question) -> bool
    {
        std::cout << question << " (Y or n): ";

        bool result = false;
        uint32_t c = 0;

        switch ((c = getInput()))
        {
            case 89:            // y // NOLINT
            case 121:           // Y // NOLINT
            case KEY_ENTER_LF:  // Line Feed
            case KEY_ENTER: result = true; break;
            default: break;
        }

        return result;
    }

    void pressEnterToContinue()
    {
        std::cout << COLOR_GREY << "Press [Enter] to continue..." << COLOR_END << "\n\n";

        while (true)
        {
            auto c = getInput();
            if (c == KEY_ENTER || c == KEY_ENTER_LF)
            {
                break;
            }
        }
    }

    auto createConversationPrompt(const std::string &who, const std::string &what, const std::string &picture)
        -> std::function<void()>
    {
        auto prompt = [who, what, picture]()
        {
            printBorder();
            std::cout << '|' << COLOR_GREEN << who << COLOR_END << '\n';
            printBorder();
            std::cout << Utils::COLOR_GREEN;
            std::cout << picture;
            std::cout << Utils::COLOR_END;
            printBorder();
            std::cout << "|\n";
            std::cout << "| " << Utils::COLOR_GREY << who << ": " << Utils::COLOR_END << what << '\n';
            std::cout << "|\n";
            printBorder();
        };
        return prompt;
    }

    auto enterName() -> const std::string
    {
        newLine();
        std::string name = "";

        std::cout << "Name: ";
        std::cin >> name;

        static constexpr auto ignoreSize = 256;

        while (name == "" || trim(name) == "" || name.length() < 1 || std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(ignoreSize, '\n');
            std::cout << "Name: ";
            std::cin >> name;
        }

        std::cin.clear();
        std::cin.ignore(ignoreSize, '\n');

        return trim(name);
    }

    void printBorder(const uint32_t length)
    {
        std::stringstream ss;
        ss << '+';
        for (uint32_t i = 0; i < length - 2; i++)
        {
            ss << '-';
        }
        ss << "+\n";
        std::cout << ss.str();
    }

    auto getCursorPosition() -> uint32_t
    {
#if defined _WIN32
        static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        GetConsoleScreenBufferInfo(hOut, &csbiInfo);
        return csbiInfo.dwCursorPosition.Y;
#else
        std::cout << "\r";
        std::cout << "\033[s";
        return 0;
#endif
    }

    void setCursorPosition([[maybe_unused]] const uint32_t x, [[maybe_unused]] const uint32_t y)
    {
#if defined _WIN32
        static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        std::cout.flush();
        COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        SetConsoleCursorPosition(hOut, coord);
#else
        std::cout << "\033[u";
#endif
    }

}  // namespace Utils
