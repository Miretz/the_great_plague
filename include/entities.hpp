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
    std::string modifierDescription;
    Attributes attributes;
    uint32_t abilityId;
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
    std::vector<uint32_t> backpack;
    std::unordered_map<std::string, uint32_t> equipped;
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
    std::vector<uint32_t> abilities;
    std::vector<StatusEffect> statusEffects;
    Inventory inventory;
};

struct GameState
{
    uint32_t areaId;
    std::vector<Hero> heroes;

    uint32_t danseaLocation;
};

/************************************************************************************************
GLOBAL LISTS
*************************************************************************************************/

const std::unordered_map<Race, RaceDetail> g_AllRaces{
    {Race::Human, {Race::Human, "Human", "The most common among the races.", "(+1 to all attributes)", {11, 11, 11, 11}, 0}},

    {Race::Repsoris, {Race::Repsoris, "Repsoris", "Identified by their reptilian features and hatred of cold.", "(+1 Strength, +1 Vitality)", {11, 10, 11, 10}, 10}},

    {Race::Ursobac, {Race::Ursobac, "Ursobac", "Their large intimidating physique makes them valuable protectors.", "(+2 Strength)", {12, 10, 10, 10}, 11}},

    {Race::Rodanto, {Race::Rodanto, "Rodanto", "They are especially proud of their prominent incisors. Their small physique allows them to hide easily.", "(+2 Dexterity)", {10, 12, 10, 10}, 12}},

    {Race::Felidae, {Race::Felidae, "Felidae", "Recognized by their cat-like eyes. Their grace and finesse is matched by none.", "(+1 Dexterity, +1 Vitality)", {10, 11, 11, 10}, 13}},

    {Race::Strigifor, {Race::Strigifor, "Strigifor", "Their huge eyes are full of wisdom and understanding, but the feathery crown on their head provides little protection.", "(+2 Intelligence)", {10, 10, 10, 12}, 14}},

    {Race::Vulpotis, {Race::Vulpotis, "Vulpotis", "Slick, cunning and opportunistic. They hide a lot of wits under their dense fur.", "(+1 Dexterity, +1 Intelligence)", {10, 11, 10, 11}, 15}},
};

const std::vector<Item> g_AllItems{
    {"Dagger", "Knife you stole from your mom's kitchen", ItemType::Dual_Wielding, {0, 0, 0, 0}, 3, 0},
    {"Short sword", "Simple short sword that anyone can use", ItemType::Melee_OneHanded, {0, 0, 0, 0}, 5, 0},
    {"Short bow", "Basic wooden bow", ItemType::Ranged_TwoHanded, {0, 0, 0, 0}, 5, 0},
    {"Wand", "A simple tree branch", ItemType::Ranged_OneHanded, {0, 0, 0, 0}, 5, 0},
    {"Long sword", "A long and heavy two-handed sword", ItemType::Melee_TwoHanded, {5, 0, 0, 0}, 8, 0},
    {"Long bow", "Long bow", ItemType::Ranged_TwoHanded, {0, 5, 0, 0}, 8, 0},
    {"Quarterstaff", "A solid broomstick", ItemType::Melee_TwoHanded, {0, 0, 0, 5}, 8, 0},

    {"Worn clothes", "Basic clothing", ItemType::Armor_Torso, {}, 0, 2},
    {"Worn boots", "Basic boots", ItemType::Armor_Legs, {}, 0, 2},
    {"Worn hood", "Simple hood", ItemType::Armor_Head, {}, 0, 2},
    {"Worn gloves", "Worn gloves", ItemType::Armor_Gloves, {}, 0, 2},
};