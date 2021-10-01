#pragma once
#ifndef utils_hpp
#define utils_hpp

#include "entities.hpp"

#include <functional>

namespace Utils
{

    const std::string COLOR_GREEN = "\033[1;32m";
    const std::string COLOR_YELLOW = "\033[1;33m";
    const std::string COLOR_GREY = "\033[2;37m";
    const std::string COLOR_END = "\033[0m";

    const uint32_t KEY_UP = 72;
    const uint32_t KEY_DOWN = 80;
    const uint32_t KEY_LEFT = 75;
    const uint32_t KEY_RIGHT = 77;
    const uint32_t KEY_ENTER = 13;
    const uint32_t KEY_ENTER_LF = 10;

    void printAttributes(const Attributes &attributes);

    void printAttributesAdjustment(const Attributes &base, const Attributes &adjustment);

    void printHeroAbilities(const std::vector<uint32_t> &abilities);

    void printItem(const uint32_t a);

    const std::string getItemString(const uint32_t a);

    const std::string getEquippedItemsString(const std::unordered_map<std::string, uint32_t> &equipped);

    const std::string getBackpack(const std::vector<uint32_t> &backpack);

    void printHeroInventory(const Inventory &inventory);

    void printHeroHeader(const std::string &name, const uint32_t level);

    void printBorderedText(const std::string &text);

    void newLine();

    void printHero(const Hero &hero);

    void printListOfHeroes(const std::vector<Hero> &heroes);

    uint32_t slider(std::function<void()> redrawFunction, const uint32_t min, const uint32_t max);

    uint32_t pickOptionFromList(std::function<void()> redrawFunction, const std::vector<std::string> &list);

    std::pair<std::vector<uint32_t>, uint32_t> pointsDistributionMenu(std::function<void()> redrawFunction, std::vector<std::pair<std::string, uint32_t>> elements, uint32_t pointsToDistribute);

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

}

#endif
