#pragma once
#ifndef entities_hpp
#define entities_hpp

#include <string>
#include <vector>
#include <unordered_map>

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

    Shield, // Round Shield, Buckler, Large Shield

    Armor_Head,
    Armor_Torso,
    Armor_Legs,
    Armor_Gloves,

    Consumable,
    Throwable, // Grenades, Throwing Knifes, Shuriken, Molotov, Oil Flask
    Scroll
};

enum class PrimaryAttribute
{
    None,
    Strength,
    Dexterity,
    Intelligence
};

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
    uint32_t constitution;
    uint32_t intelligence;
};

struct Specialties
{
    uint32_t oneHanded;
    uint32_t twoHanded;
    uint32_t ranged;
    uint32_t dualWielding;

    uint32_t terramancy;
    uint32_t necromancy;
    uint32_t hydromancy;
    uint32_t pyromancy;
    uint32_t mysticism;
};

struct RaceDetail
{
    std::string name;
    std::string description;
    uint32_t abilityId;
};

struct Item
{
    std::string name;
    std::string description;
    ItemType type;
    PrimaryAttribute primaryAttribute;

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

struct Hero
{
    std::string name;

    uint32_t health;
    uint32_t xp;

    uint32_t maxHealth;
    uint32_t level;
    uint32_t xpToLevelUp;

    uint32_t unspentAttributePoints;
    uint32_t unspentSpecialtyPoints;

    Race race;
    Controller controller;

    Attributes attributes;
    Specialties specialties;
    std::vector<uint32_t> abilities;
    std::vector<uint32_t> statusEffects;
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
    {Race::Human, {"Human", "The most common among the races.", 16}},
    {Race::Repsoris, {"Repsoris", "Identified by their reptilian features and hatred of cold.", 10}},
    {Race::Ursobac, {"Ursobac", "Their large intimidating physique makes them valuable protectors.", 11}},
    {Race::Rodanto, {"Rodanto", "They are especially proud of their prominent incisors. Their small physique allows them to hide easily.", 12}},
    {Race::Felidae, {"Felidae", "Recognized by their cat-like eyes. Their grace and finesse is matched by none.", 13}},
    {Race::Strigifor, {"Strigifor", "Their huge eyes are full of wisdom and understanding, but the feathery crown on their head provides little protection.", 14}},
    {Race::Vulpotis, {"Vulpotis", "Slick, cunning and opportunistic. They hide a lot of wits under their dense fur.", 15}},
};

const std::vector<Item> g_AllItems{
    {"Dagger",
     "Knife you stole from your mom's kitchen",
     ItemType::Melee_OneHanded,
     PrimaryAttribute::Dexterity,
     {0, 0, 0, 0},
     5,
     0},
    {"Short sword",
     "Simple short sword that anyone can use",
     ItemType::Melee_OneHanded,
     PrimaryAttribute::Strength,
     {0, 0, 0, 0},
     10,
     0},
    {"Short bow",
     "Basic wooden bow",
     ItemType::Ranged_TwoHanded,
     PrimaryAttribute::Dexterity,
     {0, 0, 0, 0},
     10,
     0},
    {"Wand",
     "A simple tree branch",
     ItemType::Ranged_OneHanded,
     PrimaryAttribute::Intelligence,
     {0, 0, 0, 0},
     10,
     0},
    {"Long sword",
     "A long and heavy two-handed sword",
     ItemType::Melee_TwoHanded,
     PrimaryAttribute::Strength,
     {5, 0, 0, 0},
     20,
     0},
    {"Long bow",
     "A very strong bow",
     ItemType::Ranged_TwoHanded,
     PrimaryAttribute::Dexterity,
     {0, 5, 0, 0},
     20,
     0},
    {"Quarterstaff",
     "A simple broomstick",
     ItemType::Melee_TwoHanded,
     PrimaryAttribute::Dexterity,
     {0, 0, 0, 5},
     20,
     0},
    {"Worn clothes",
     "Basic clothing",
     ItemType::Armor_Torso,
     PrimaryAttribute::None,
     {},
     0,
     2},
    {"Worn boots",
     "Basic boots",
     ItemType::Armor_Legs,
     PrimaryAttribute::None,
     {},
     0,
     2},
    {"Worn hood",
     "Simple hood",
     ItemType::Armor_Head,
     PrimaryAttribute::None,
     {},
     0,
     2},
    {"Worn gloves",
     "Worn gloves",
     ItemType::Armor_Gloves,
     PrimaryAttribute::None,
     {},
     0,
     2},
};

#endif