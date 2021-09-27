#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

/************************************************************************************************
ENUMS
*************************************************************************************************/

enum class Race
{
    Repsoris,
    Ursobac,
    Rodanto,
    Felidae,
    Strigifor,
    Vulpotis,
};

enum class ItemType
{
    Melee_OneHanded, // Sword, Hatched, Mace, Scimitar, Sabre, Katana
    Melee_TwoHanded, // Hammer, Long Sword, Pike, Quarterstaff, Battle Axe

    Ranged_OneHanded, // Wand, Pistol, Hand crossbow
    Ranged_TwoHanded, // Bow, Crossbow, Long Bow, Musket

    Shield,        // Round Shield, Buckler, Large Shield
    Dual_Wielding, // Dagger,

    Armor_Head,
    Armor_Torso,
    Armor_Legs,
    Armor_Gloves,

    Consumable,
    Throwable, // Grenades, Throwing Knifes, Shuriken, Molotov, Oil Flask
    Scroll
};

enum class EquipmentSlot
{
    MainHand,
    Offhand,
    Torso,
    Head,
    Legs,
    Gloves
};

const std::vector<std::string> equipmentSlotNames{"Main Hand", "Offhand", "Torso", "Head", "Legs", "Gloves"};

enum class Controller
{
    Player,
    AI_Friendly,
    AI_Enemy,
};

/************************************************************************************************
CLASSES
*************************************************************************************************/

struct Attributes
{
    int strength;
    int dexterity;
    int vitality;
    int intelligence;
};

struct RaceDetail
{
    Race id;
    std::string name;
    std::string description;
    Attributes attributes;
};

struct Item
{
    std::string name;
    std::string description;
    ItemType type;

    Attributes requirements;
    int damage;
    int armor;
};

struct Inventory
{
    std::vector<Item> backpack;
    std::unordered_map<std::string, Item> equipped;
};

struct Hero;

struct Ability
{
    std::string name;
    std::string description;
    std::function<void(Hero &, Hero &)> action;
};

struct StatusEffect
{
    std::string name;
    int expireInTurns;
    std::function<void(Hero &)> execute;
};

struct Hero
{
    std::string name;
    int health;
    int xp;

    int maxHealth;
    int level;
    int xpToLevelUp;
    int unspentPoints;

    Race race;
    Controller controller;

    Attributes attributes;
    std::vector<Ability> abilities;
    std::vector<StatusEffect> statusEffects;
    Inventory inventory;
};

/************************************************************************************************
GLOBAL LISTS
*************************************************************************************************/

const std::unordered_map<Race, RaceDetail> g_AllRaces{
    {Race::Repsoris, {Race::Repsoris, "Repsoris", "Identified by their reptilian features and hatred for cold.", {8, 6, 2, 1}}},
    {Race::Ursobac, {Race::Ursobac, "Ursobac", "Their large intimidating physique makes them valuable protectors.", {12, 1, 6, 1}}},
    {Race::Rodanto, {Race::Rodanto, "Rodanto", "They are especially proud of their prominent incisors, although they like to lurk in the shadows.", {1, 14, 2, 3}}},
    {Race::Felidae, {Race::Felidae, "Felidae", "Their grace and finesse is matched by none. Some believe that they have more than one life.", {1, 11, 4, 4}}},
    {Race::Strigifor, {Race::Strigifor, "Strigifor", "Their huge eyes are full of wisdom and understanding, but the feathery crown on their head provides little protection.", {1, 1, 1, 15}}},
    {Race::Vulpotis, {Race::Vulpotis, "Vulpotis", "Slick, cunning and opportunistic. They hide a lot of wits under their dense fur.", {1, 4, 2, 13}}},
};