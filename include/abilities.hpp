#pragma once

#include "entities.hpp"

namespace Abilities
{
    void a_FirstAid(Hero &caster, Hero &target);
    void a_Evasion(Hero &caster, Hero &target);
    void a_Knockdown(Hero &caster, Hero &target);
    void a_VampiricBite(Hero &caster, Hero &target);
    void a_MagicShield(Hero &caster, Hero &target);
    void a_SummonImp(Hero &caster, Hero &target);
    void a_RaiseCorpse(Hero &caster, Hero &target);


    const std::vector<Ability> startingAbilities{
        {"First Aid", "Heal yourself or a friendly unit (revives fallen allies).", a_FirstAid},
        {"Evasion", "Lower the chances of getting hit by 90%.", a_Evasion},
        {"Knockdown", "Knocks down an enemy causing them to lose turn.", a_Knockdown},
        {"Vampiric Bite", "Attack enemy with a vicious bite (restores health).", a_VampiricBite},
        {"Magic Shield", "Grants additional armor to character.", a_MagicShield},
        {"Summon Imp", "Summons an Imp from a different dimension to aid you in battle.", a_SummonImp},
        {"Raise Corpse", "Turns a nearby corpse into a zombie", a_RaiseCorpse},
    };

    Ability pickStartingAbility();
    void learnAbility(Hero &hero, Ability ability);


}