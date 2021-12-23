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
    inline constexpr auto kAbilitiesFile = "assets/abilities/abilities.txt";

    [[nodiscard]] auto getAbility(const std::string &id) -> std::optional<Ability>;

    [[nodiscard]] auto loadFromFile() -> std::unordered_map<std::string, Ability>;

    void learnAbility(Hero &hero, const std::string &id);

    void executeAbility(const std::string &id, Hero &caster, Hero &target, Combat &combat);

    // ability scripts
    void f_FirstAid(const Hero &caster, Hero &target, Combat &combat);
    void f_Maul(const Hero &caster, Hero &target, Combat &combat);
    void f_LifeDrain(Hero &caster, Hero &target, Combat &combat);
    void f_SummonDog(Hero &caster, Hero &target, Combat &combat);
    void f_HailStorm(const Hero &caster, Hero &target, Combat &combat);
    void f_ReviveDead(const Hero &caster, Hero &target, Combat &combat);

    const std::unordered_map<std::string, std::function<void(Hero &, Hero &, Combat &)>> mappedFunctions{
        { "f_FirstAid", [](Hero &caster, Hero &target, Combat &combat) { f_FirstAid(caster, target, combat); } },
        { "f_Maul", [](Hero &caster, Hero &target, Combat &combat) { f_Maul(caster, target, combat); } },
        { "f_LifeDrain", [](Hero &caster, Hero &target, Combat &combat) { f_LifeDrain(caster, target, combat); } },
        { "f_SummonDog", [](Hero &caster, Hero &target, Combat &combat) { f_SummonDog(caster, target, combat); } },
        { "f_HailStorm", [](Hero &caster, Hero &target, Combat &combat) { f_HailStorm(caster, target, combat); } },
        { "f_ReviveDead", [](Hero &caster, Hero &target, Combat &combat) { f_ReviveDead(caster, target, combat); } }
    };
}  // namespace Abilities

#endif