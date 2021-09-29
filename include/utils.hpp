#pragma once

#include "entities.hpp"

#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_GREY "\033[2;37m"
#define COLOR_END "\033[0m"

constexpr uint32_t KEY_UP = 72;
constexpr uint32_t KEY_DOWN = 80;
constexpr uint32_t KEY_LEFT = 75;
constexpr uint32_t KEY_RIGHT = 77;
constexpr uint32_t KEY_ENTER = 13;
constexpr uint32_t KEY_ENTER_LF = 10;

#define SAVE_FILE "save.txt"

std::string getEquipmentSlotName(EquipmentSlot eSlot);

void printAttributes(Attributes attributes);

void printAttributesAdjustment(Attributes base, Attributes adjustment);

void printHeroAbilities(const std::vector<uint32_t> &abilities);

void printItem(uint32_t a);

void printListOfHeroes(const std::vector<Hero> &heroes);

std::string getItemString(uint32_t a);

std::string getEquippedItemsString(const std::unordered_map<std::string, uint32_t> &equipped);

std::string getBackpack(const std::vector<uint32_t> &backpack);

void printHeroInventory(const Inventory &inventory);

void printHeroHeader(const std::string &name, const uint32_t level);

void printHero(Hero &hero);

uint32_t slider(std::function<void()> redrawFunction, uint32_t min, uint32_t max);

uint32_t pickOptionFromList(std::function<void()> redrawFunction, const std::vector<std::string> &list);

std::pair<std::vector<uint32_t>, uint32_t> pointsDistributionMenu(std::function<void()> redrawFunction, std::vector<std::pair<std::string, uint32_t>> elements, uint32_t pointsToDistribute);

void clearScreen();

std::string trim(std::string s);

void printIntro();

bool askConfirmation(const std::string &question);

void pressEnterToContinue();

std::function<void()> createConversationPrompt(std::string who, std::string what, std::string picture);

std::string enterName();

bool isNameAlreadyInUse(std::string name, const std::vector<Hero> &heroes);

void printBorder(uint32_t length);

uint32_t getCursorPosition();

void setCursorPosition(uint32_t x, uint32_t y);

std::string serializeHero(Hero hero);

Hero deserializeHero(std::string serialized);

void saveGame(std::vector<Hero> &heroes, uint32_t areaId);

std::pair<uint32_t, std::vector<Hero>> loadGame();
