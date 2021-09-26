#pragma once

#include "entities.hpp"

std::string getEquipmentSlotName(EquipmentSlot eSlot);

void printAttributes(Attributes attributes);

void printHeroAbilities(const std::vector<Ability> &abilities);

void printItem(Item a);

void printEquippedItems(const std::unordered_map<std::string, Item> &equipped);

void printBackpack(const std::vector<Item> &backpack);

void printHeroInventory(const Inventory &inventory);

void printHero(Hero hero);

unsigned int pickOption(unsigned int numberOfOptions);

unsigned int pickOptionFromList(const std::vector<std::string>& list);

void clearScreen();

std::string trim(std::string s);

void printIntro();

bool askConfirmation(const std::string &question);

std::string enterName();

bool isNameAlreadyInUse(std::string name, const std::vector<Hero> &heroes);