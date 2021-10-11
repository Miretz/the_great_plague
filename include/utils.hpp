#pragma once
#ifndef utils_hpp
#define utils_hpp

#include "entities.hpp"

#include <functional>
#include <random>

using PointsMenuInput = std::vector<std::tuple<std::string, std::string, uint32_t>>;

namespace Utils
{

    const std::string COLOR_GREEN = "\033[1;32m";
    const std::string COLOR_YELLOW = "\033[1;33m";
    const std::string COLOR_GREY = "\033[2;37m";
    const std::string COLOR_RED = "\33[1;31m";
    const std::string COLOR_END = "\033[0m";
    const std::string COLOR_CYAN = "\033[0;36m";
    const std::string COLOR_CYAN_LIGHT = "\033[1;36m";

#if defined _WIN32
    const uint32_t KEY_UP = 72;
    const uint32_t KEY_DOWN = 80;
    const uint32_t KEY_LEFT = 75;
    const uint32_t KEY_RIGHT = 77;
    const uint32_t KEY_ENTER = 13;
    const uint32_t KEY_ENTER_LF = 10;
#else
    const char KEY_UP = 'A';
    const char KEY_DOWN = 'B';
    const char KEY_LEFT = 'D';
    const char KEY_RIGHT = 'C';
    const char KEY_ENTER = '\r';
    const char KEY_ENTER_LF = '\n';
#endif

    const std::string addPlus(uint32_t value);

    void printAttributes(const Attributes &attributes);

    void printAttributesAdjustment(const Attributes &base, const Attributes &adjustment);

    void printSpecialties(const Specialties &specialties);

    void printSpecialtiesAdjustment(const Specialties &base, const Specialties &adjustment);

    void printHeroAbilities(const std::vector<std::string> &abilities);

    const std::string getItemString(const uint32_t a, bool describe);

    const std::string getEquippedItemsString(const std::unordered_map<std::string, uint32_t> &equipped);

    const std::string getBackpack(const std::vector<uint32_t> &backpack);

    void printHeroInventory(const Inventory &inventory);

    void printHeroHeader(const std::string &name, const uint32_t level);
    
    void printCombatHeroHeader(const Hero &hero);

    void printBorderedText(const std::string &text);

    void printSpacedText(const std::string &text);

    void newLine();

    const std::string getFullPhysicalDamage(const Hero &hero);

    void printFullPhysicalDamage(const Hero &hero);

    void printHero(const Hero &hero);

    void printListOfHeroes(const std::vector<Hero> &heroes);

    uint32_t slider(std::function<void()> redrawFunction, const uint32_t min, const uint32_t max);

    uint32_t pickOptionFromList(std::function<void()> redrawFunction, const std::vector<std::string> &list);

    std::pair<std::vector<uint32_t>, uint32_t> pointsDistributionMenu(std::function<void()> redrawFunction, PointsMenuInput elements, uint32_t pointsToDistribute);

    void clearScreen();

    std::string trim(std::string s);

    void printIntro();

    bool askConfirmation(const std::string &question);

    void pressEnterToContinue();

    std::function<void()> createConversationPrompt(const std::string &who, const std::string &what, const std::string &picture);

    const std::string enterName();

    void printBorder(const uint32_t length);

    uint32_t getCursorPosition();

    void setCursorPosition(const uint32_t x, const uint32_t y);

    uint32_t getInput();

}

#endif
