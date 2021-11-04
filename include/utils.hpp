#pragma once
#ifndef utils_hpp
#define utils_hpp

#include <functional>
#include <random>

#include "entities.hpp"

using PointsMenuInput = std::vector<std::tuple<std::string, std::string, uint32_t>>;

namespace Utils
{
    inline constexpr auto kColorGreen = "\033[1;32m";
    inline constexpr auto kColorYellow = "\033[1;33m";
    inline constexpr auto kColorGrey = "\033[2;37m";
    inline constexpr auto kColorRed = "\33[1;31m";
    inline constexpr auto kColorEnd = "\033[0m";
    inline constexpr auto kColorCyan = "\033[0;36m";
    inline constexpr auto KColorCyanLight = "\033[1;36m";

#if defined _WIN32
    inline constexpr uint32_t kKeyUp = 72;
    inline constexpr uint32_t kKeyDown = 80;
    inline constexpr uint32_t kKeyLeft = 75;
    inline constexpr uint32_t kKeyRight = 77;
    inline constexpr uint32_t kKeyEnter = 13;
    inline constexpr uint32_t kKeyEnterLineFeed = 10;
#else
    inline constexpr char kKeyUp = 'A';
    inline constexpr char kKeyDown = 'B';
    inline constexpr char kKeyLeft = 'D';
    inline constexpr char kKeyRight = 'C';
    inline constexpr char kKeyEnter = '\r';
    inline constexpr char kKeyEnterLineFeed = '\n';
#endif

    inline constexpr auto kPaddingLeft = 16;
    inline constexpr auto kPaddingRight = 6;

    void initConsole();

    auto addPlus(uint32_t value) -> const std::string;

    void printAttributes(const Attributes &attributes);

    void printAttributesAdjustment(const Attributes &base, const Attributes &adjustment);

    void printSpecialties(const Specialties &specialties);

    void printSpecialtiesAdjustment(const Specialties &base, const Specialties &adjustment);

    void printHeroAbilities(const std::vector<std::string> &abilities);

    auto getItemString(const uint32_t a, bool describe) -> const std::string;

    auto getAbilityString(const Ability &ability) -> const std::string;

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

    auto pointsDistributionMenu(
        const std::function<void()> &redrawFunction,
        PointsMenuInput elements,
        uint32_t pointsToDistribute) -> std::pair<std::vector<uint32_t>, uint32_t>;

    void clearScreen();

    auto trim(const std::string &s) -> std::string;

    void printIntro();

    auto askConfirmation(const std::string &question) -> bool;

    void pressEnterToContinue();

    auto createConversationPrompt(const std::string &who, const std::string &what, const std::string &picture)
        -> std::function<void()>;

    auto enterName() -> const std::string;

    void printBorder(const uint32_t length = 130);

    auto getCursorPosition() -> uint32_t;

    void setCursorPosition(const uint32_t x, const uint32_t y);

    auto getInput() -> uint32_t;

}  // namespace Utils

#endif
