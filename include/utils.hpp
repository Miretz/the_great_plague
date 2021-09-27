#pragma once

#include "entities.hpp"

#include <conio.h>

#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_END "\033[0m"

constexpr int KEY_UP = 72;
constexpr int KEY_DOWN = 80;
constexpr int KEY_LEFT = 75;
constexpr int KEY_RIGHT = 77;
constexpr int KEY_ENTER = 13;
constexpr int KEY_ENTER_LF = 10;

std::string getEquipmentSlotName(EquipmentSlot eSlot);

void printAttributes(Attributes attributes);

void printAttributesAdjustment(Attributes base, Attributes adjustment);

void printHeroAbilities(const std::vector<Ability> &abilities);

void printItem(Item a);

std::string getItemString(Item a);

std::string getEquippedItemsString(const std::unordered_map<std::string, Item> &equipped);

std::string getBackpack(const std::vector<Item> &backpack);

void printHeroInventory(const Inventory &inventory);

void printHeroHeader(const std::string& name, const int level);

void printHero(Hero &hero);

int slider(std::function<void()> redrawFunction, int min, int max);

unsigned int pickOptionFromList(std::function<void()> redrawFunction, const std::vector<std::string> &list);

std::pair<std::vector<int>, int> pointsDistributionMenu(std::function<void()> redrawFunction, std::vector<std::pair<std::string, int>> elements, int pointsToDistribute);

void clearScreen();

std::string trim(std::string s);

void printIntro();

bool askConfirmation(const std::string &question);

std::string enterName();

bool isNameAlreadyInUse(std::string name, const std::vector<Hero> &heroes);

void printBorder(int length);