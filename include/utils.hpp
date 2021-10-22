#pragma once
#ifndef utils_hpp
#define utils_hpp

#include <functional>
#include <random>

#include "entities.hpp"

using PointsMenuInput = std::vector<std::tuple<std::string, std::string, uint32_t>>;

namespace Utils
{
    static constexpr auto COLOR_GREEN = "\033[1;32m";
    static constexpr auto COLOR_YELLOW = "\033[1;33m";
    static constexpr auto COLOR_GREY = "\033[2;37m";
    static constexpr auto COLOR_RED = "\33[1;31m";
    static constexpr auto COLOR_END = "\033[0m";
    static constexpr auto COLOR_CYAN = "\033[0;36m";
    static constexpr auto COLOR_CYAN_LIGHT = "\033[1;36m";

#if defined _WIN32
    static constexpr uint32_t KEY_UP = 72;
    static constexpr uint32_t KEY_DOWN = 80;
    static constexpr uint32_t KEY_LEFT = 75;
    static constexpr uint32_t KEY_RIGHT = 77;
    static constexpr uint32_t KEY_ENTER = 13;
    static constexpr uint32_t KEY_ENTER_LF = 10;
#else
    static constexpr char KEY_UP = 'A';
    static constexpr char KEY_DOWN = 'B';
    static constexpr char KEY_LEFT = 'D';
    static constexpr char KEY_RIGHT = 'C';
    static constexpr char KEY_ENTER = '\r';
    static constexpr char KEY_ENTER_LF = '\n';
#endif

    static constexpr auto kPaddingLeft = 16;
    static constexpr auto kPaddingRight = 6;

    void maximizeConsole();

    auto addPlus(uint32_t value) -> const std::string;

    void printAttributes(const Attributes &attributes);

    void printAttributesAdjustment(const Attributes &base, const Attributes &adjustment);

    void printSpecialties(const Specialties &specialties);

    void printSpecialtiesAdjustment(const Specialties &base, const Specialties &adjustment);

    void printHeroAbilities(const std::vector<std::string> &abilities);

    auto getItemString(const uint32_t a, bool describe) -> const std::string;

    auto getEquippedItemsString(const std::unordered_map<std::string, uint32_t> &equipped) -> const std::string;

    auto getBackpack(const std::vector<uint32_t> &backpack) -> const std::string;

    void printHeroInventory(const Inventory &inventory);

    void printHeroHeader(const std::string &name, const uint32_t level);

    void printCombatHeroHeader(const Hero &hero);

    void printBorderedText(const std::string &text);

    void printBorderedTextWithColor(const std::string &text, const std::string &color);

    auto colorize(const std::string &text, const std::string &color) -> const std::string;

    void printSpacedText(const std::string &text);

    void printArea(const std::string &image, const std::string &description);

    void printCombatStart(const std::string &description);

    void newLine();

    auto getFullPhysicalDamage(const Hero &hero) -> const std::string;

    void printFullPhysicalDamage(const Hero &hero);

    void printHero(const Hero &hero);

    void printListOfHeroes(const std::vector<Hero> &heroes);

    auto slider(const std::function<void()> &redrawFunction, const uint32_t min, const uint32_t max) -> uint32_t;

    auto pickOptionFromList(const std::function<void()> &redrawFunction, const std::vector<std::string> &list) -> uint32_t;

    auto pointsDistributionMenu(const std::function<void()> &redrawFunction, PointsMenuInput elements, uint32_t pointsToDistribute) -> std::pair<std::vector<uint32_t>, uint32_t>;

    void clearScreen();

    auto trim(const std::string &s) -> std::string;

    void printIntro();

    auto askConfirmation(const std::string &question) -> bool;

    void pressEnterToContinue();

    auto createConversationPrompt(const std::string &who, const std::string &what, const std::string &picture) -> std::function<void()>;

    auto enterName() -> const std::string;

    void printBorder(const uint32_t length = 130);

    auto getCursorPosition() -> uint32_t;

    void setCursorPosition(const uint32_t x, const uint32_t y);

    auto getInput() -> uint32_t;

}  // namespace Utils

#endif
