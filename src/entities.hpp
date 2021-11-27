#ifndef entities_hpp
#define entities_hpp

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

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
    Canis,
};

enum class ItemType
{
    Melee_OneHanded,  // Sword, Hatched, Mace, Scimitar, Sabre, Katana
    Melee_TwoHanded,  // Hammer, Long Sword, Pike, Quarterstaff, Battle Axe

    Ranged_OneHanded,  // Wand, Pistol, Hand crossbow
    Ranged_TwoHanded,  // Bow, Crossbow, Long Bow, Musket

    Shield,  // Round Shield, Buckler, Large Shield

    Armor_Head,
    Armor_Torso,
    Armor_Legs,
    Armor_Gloves,

    Consumable,
    Throwable,  // Grenades, Throwing Knifes, Shuriken, Molotov, Oil Flask
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

enum class AbilityType
{
    Damage,        // Maul, Precision shot, Life Drain, Pounce
    Healing,       // First Aid,
    Summoning,     // Summon Familiar, Man's best friend, Raise corpse
    StatusEffect,  // Knockdown, Evasion, Magic Shield, Poison
    AoE_Status,
    AoE_Damage,
    AoE_Healing,
};

enum class Target
{
    Self,
    Friendly,
    Enemy
};

enum class StatusEffectType
{
    SkipTurn,    // Knockdown
    Damage,      // Poision, Burning, ...
    Heal,        // Restoration
    Protection,  // Magic Armor
    Invisibility,
};

/************************************************************************************************
OBJECTS
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

    uint32_t mysticism;
    uint32_t terramancy;
    uint32_t hydromancy;
    uint32_t pyromancy;
    uint32_t aeromancy;
};

struct RaceDetail
{
    std::string name;
    std::string description;
    std::string abilityId;
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

struct StatusEffect
{
    StatusEffectType type;
    std::string name;
    uint32_t turnsLeft;
    uint32_t specialValue;
};

struct Hero
{
    std::string uniqueId;
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
    std::vector<std::string> abilities;
    Inventory inventory;

    // not serialized
    std::vector<StatusEffect> statusEffects;
    uint32_t actionPoints;
};

struct GameState
{
    uint32_t areaId;
    std::vector<Hero> heroes;

    std::unordered_map<std::string, uint32_t> stateInfo;
};

struct Combat
{
    uint32_t turn;
    uint32_t currentHero;
    std::vector<Hero> turnQueue;
    std::vector<Hero> dead;
    std::vector<Hero> spawnQueue;
};

struct Ability
{
    std::string id;
    std::string name;
    std::string description;
    std::string mapping;
    AbilityType type;
    Target target;
    uint32_t actionPoints;
};

struct ConversationLine
{
    std::string label;
    std::string who;
    std::string text;
    std::vector<std::string> jumps;
};

/************************************************************************************************
GLOBAL LISTS
*************************************************************************************************/

const std::unordered_map<Race, RaceDetail> g_AllRaces{
    { Race::Human, { "Human", "The most common among the races.", "race_human" } },
    { Race::Repsoris, { "Repsoris", "Identified by their reptilian features and hatred of cold.", "race_repsoris" } },
    { Race::Ursobac, { "Ursobac", "Their large intimidating physique makes them valuable protectors.", "race_ursobac" } },
    { Race::Rodanto,
      { "Rodanto",
        "They are especially proud of their prominent incisors. Their small physique allows them to hide easily.",
        "race_rodanto" } },
    { Race::Felidae,
      { "Felidae", "Recognized by their cat-like eyes. Their grace and finesse is matched by none.", "race_felidae" } },
    { Race::Strigifor,
      { "Strigifor",
        "Their huge eyes are full of wisdom and understanding, but the feathery crown on their head provides little "
        "protection.",
        "race_strigifor" } },
    { Race::Vulpotis,
      { "Vulpotis", "Slick, cunning and opportunistic. They hide a lot of wits under their dense fur.", "race_vulpotis" } },
    { Race::Canis, { "Canis", "A domesticated descendant of the grey wolf.", "" } },
};

const std::unordered_map<std::string, StatusEffect> g_StatusEffects{
    { "s_Knockdown", { StatusEffectType::SkipTurn, "Knocked Down", 1, 0 } },
    { "s_Poison", { StatusEffectType::Damage, "Poisoned", 2, 10 } },
    { "s_Evasion", { StatusEffectType::Protection, "Evading", 3, 15 } },
    { "s_Camouflage", { StatusEffectType::Invisibility, "Invisible", 3, 10 } },
    { "s_MagicShield", { StatusEffectType::Protection, "M.Shield", 1, 5 } },
};

const std::vector<Item> g_AllItems{
    { "Dagger",
      "Knife you stole from your mom's kitchen",
      ItemType::Melee_OneHanded,
      PrimaryAttribute::Dexterity,
      { 0, 0, 0, 0 },
      5,
      0 },
    { "Short sword",
      "Simple short sword that anyone can use",
      ItemType::Melee_OneHanded,
      PrimaryAttribute::Strength,
      { 0, 0, 0, 0 },
      10,
      0 },
    { "Short bow", "Basic wooden bow", ItemType::Ranged_TwoHanded, PrimaryAttribute::Dexterity, { 0, 0, 0, 0 }, 10, 0 },
    { "Wand", "A simple tree branch", ItemType::Ranged_OneHanded, PrimaryAttribute::Intelligence, { 0, 0, 0, 0 }, 10, 0 },
    { "Long sword",
      "A long and heavy two-handed sword",
      ItemType::Melee_TwoHanded,
      PrimaryAttribute::Strength,
      { 5, 0, 0, 0 },
      20,
      0 },
    { "Long bow", "A very strong bow", ItemType::Ranged_TwoHanded, PrimaryAttribute::Dexterity, { 0, 5, 0, 0 }, 20, 0 },
    { "Quarterstaff",
      "A simple broomstick",
      ItemType::Melee_TwoHanded,
      PrimaryAttribute::Intelligence,
      { 0, 0, 0, 5 },
      20,
      0 },
    { "Worn clothes", "Basic clothing", ItemType::Armor_Torso, PrimaryAttribute::None, {}, 0, 2 },
    { "Worn boots", "Basic boots", ItemType::Armor_Legs, PrimaryAttribute::None, {}, 0, 2 },
    { "Worn hood", "Simple hood", ItemType::Armor_Head, PrimaryAttribute::None, {}, 0, 2 },
    { "Worn gloves", "Worn gloves", ItemType::Armor_Gloves, PrimaryAttribute::None, {}, 0, 2 },
};

#endif