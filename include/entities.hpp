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
    Human,
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
    uint32_t strength;
    uint32_t dexterity;
    uint32_t vitality;
    uint32_t intelligence;
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
    uint32_t damage;
    uint32_t armor;
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
    uint32_t expireInTurns;
    std::function<void(Hero &)> execute;
};

struct Hero
{
    std::string name;

    uint32_t health;
    uint32_t xp;

    uint32_t maxHealth;
    uint32_t level;
    uint32_t xpToLevelUp;
    uint32_t unspentPoints;

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
    {Race::Human, {Race::Human, "Human", "The most common among the races.", {5, 5, 5, 5}}},
    {Race::Repsoris, {Race::Repsoris, "Repsoris", "Identified by their reptilian features and hatred for cold.", {8, 6, 2, 1}}},
    {Race::Ursobac, {Race::Ursobac, "Ursobac", "Their large intimidating physique makes them valuable protectors.", {12, 1, 6, 1}}},
    {Race::Rodanto, {Race::Rodanto, "Rodanto", "They are especially proud of their prominent incisors. Their small physique allows them to hide easily.", {1, 14, 2, 3}}},
    {Race::Felidae, {Race::Felidae, "Felidae", "Recognized by their cat-like eyes. Their grace and finesse is matched by none.", {1, 11, 4, 4}}},
    {Race::Strigifor, {Race::Strigifor, "Strigifor", "Their huge eyes are full of wisdom and understanding, but the feathery crown on their head provides little protection.", {1, 1, 1, 15}}},
    {Race::Vulpotis, {Race::Vulpotis, "Vulpotis", "Slick, cunning and opportunistic. They hide a lot of wits under their dense fur.", {1, 4, 2, 13}}},
};