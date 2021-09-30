#pragma once
#ifndef abilities_hpp
#define abilities_hpp

#include <vector>
#include <string>
#include <functional>

struct Hero;

namespace Abilities
{
    void a_FirstAid(Hero &caster, Hero &target);
    void a_LifeDrain(Hero &caster, Hero &target);

    struct Ability
    {
        std::string name;
        std::string description;
        std::function<void(Hero &, Hero &)> action;
    };

    const std::vector<Ability> allAbilities{
        {"None", "None", nullptr},
        {"First Aid", "Heal yourself or a friendly unit (revives fallen allies).", a_FirstAid},
        {"Evasion", "Lower the chances of getting hit by 90%.", nullptr},
        {"Knockdown", "Knocks down an enemy causing them to lose turn.", nullptr},
        {"Life Drain", "Drain the health of the enemy restoring your health in the process.", a_LifeDrain},
        {"Magic Shield", "Grants additional armor to character.", nullptr},
        {"Summon Familiar", "Summons a creature from a different dimension to aid you in battle.", nullptr},
        {"Raise Corpse", "Turns a nearby corpse into a zombie.", nullptr},
        {"Camouflage", "Target is invisble. Can't be attacked by enemies.", nullptr},
        {"Energy Blast", "Shoot a projectile of magical energy.", nullptr},

        // Race specific
        {"Poisonous Touch", "Scratch your opponent with your poisonous claws.", nullptr},
        {"Maul", "Hit the opponent twice with a strong attack.", nullptr},
        {"Vicious Bite", "Bite your enemy and heal yourself in the process.", nullptr},
        {"Precision Strike", "Precise attack with a high chance of a Critical Hit.", nullptr},
        {"Gust", "Command the wind to strike to enemy with a powerful blast.", nullptr},
        {"Pounce", "Sudden attack that knocks down the enemy.", nullptr},
    };

    void learnAbility(Hero &hero, uint32_t abilityId);
}

#endif