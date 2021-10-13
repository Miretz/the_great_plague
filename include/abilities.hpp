#pragma once
#ifndef abilities_hpp
#define abilities_hpp

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


struct Hero;
struct Combat;
struct Ability;

namespace Abilities
{
    std::optional<Ability> getAbility(const std::string &id);

    void init();

    void learnAbility(Hero &hero, const std::string &id);

    void executeAbility(const std::string &id, Hero &caster, Hero &target, Combat &combat);

    // ability scripts
    void f_FirstAid(Hero &caster, Hero &target, Combat &combat);
    void f_Maul(Hero &caster, Hero &target, Combat &combat);
    void f_LifeDrain(Hero &caster, Hero &target, Combat &combat);
    void f_SummonDog(Hero &caster, Hero &target, Combat &combat);
    void f_HailStorm(Hero &caster, Hero &target, Combat &combat);
    void f_ReviveDead(Hero &caster, Hero &target, Combat &combat);

    const std::unordered_map<std::string, std::function<void(Hero &, Hero &, Combat &)>> mappedFunctions{
        { "f_FirstAid", f_FirstAid },   { "f_Maul", f_Maul },           { "f_LifeDrain", f_LifeDrain },
        { "f_SummonDog", f_SummonDog }, { "f_HailStorm", f_HailStorm }, { "f_ReviveDead", f_ReviveDead }
    };
}  // namespace Abilities

#endif