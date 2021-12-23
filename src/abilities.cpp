#include "abilities.hpp"

#include <optional>

#include "characters.hpp"
#include "dice.hpp"
#include "entities.hpp"
#include "files.hpp"
#include "inventory_manager.hpp"
#include "utils.hpp"

namespace Abilities
{
    std::unordered_map<std::string, Ability> loadedAbilities;  // NOLINT

    auto getAbility(std::string_view id) -> std::optional<Ability>
    {
        if (id.empty())
        {
            return std::nullopt;
        }
        return loadedAbilities.at(std::string{ id });
    }

    void init()
    {
        const auto abilitiesFromFile = Files::loadAbilities(kAbilitiesFile);
        for (const auto &ab : abilitiesFromFile)
        {
            loadedAbilities[ab.id] = ab;
        }
    }

    void learnAbility(Hero &hero, std::string_view id)
    {
        if (getAbility(id).has_value())
        {
            hero.abilities.emplace_back(id);
        }
    }

    void executeAbility(std::string_view id, Hero &caster, Hero &target, Combat &combat)
    {
        if (getAbility(id).has_value())
        {
            const auto ability = getAbility(id).value();

            if (ability.type == AbilityType::StatusEffect || ability.type == AbilityType::AoE_Status)
            {
                const auto &currentStatusEffect = g_StatusEffects.at(ability.mapping);

                // prevent applying twice
                auto existingStatusEffect = std::find_if(
                    begin(target.statusEffects),
                    end(target.statusEffects),
                    [&](const auto &a) { return currentStatusEffect.name == a.name; });
                if (existingStatusEffect != std::end(target.statusEffects))
                {
                    (*existingStatusEffect).turnsLeft = currentStatusEffect.turnsLeft;
                    return;
                }

                target.statusEffects.push_back(currentStatusEffect);
            }
            else
            {
                auto f = mappedFunctions.at(ability.mapping);
                f(caster, target, combat);
            }
        }
    }

    void f_Maul(const Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
    {
        auto damage = 5 + caster.level * 10;  // NOLINT
        Characters::takeDamage(target, damage);
    }

    void f_FirstAid(const Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
    {
        if (target.controller == Controller::Player || target.controller == Controller::AI_Friendly)
        {
            // level * 5
            uint32_t newHealth = 20 + target.health + caster.level * 5;  // NOLINT
            if (newHealth >= target.maxHealth)
            {
                target.health = target.maxHealth;
            }
            else
            {
                target.health = newHealth;
            }
        }
    }

    void f_LifeDrain(Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
    {
        // take level*5 from enemy health
        uint32_t healthStolen = 10 + caster.level * 5;  // NOLINT

        // if enemy has less than level*5 steal only remaining HP
        if (target.health <= healthStolen)
        {
            healthStolen = target.health;
            target.health = 0;
        }
        else
        {
            target.health -= healthStolen;
        }

        // add the stolen health to ourselves
        uint32_t newHealth = caster.health + healthStolen;
        if (newHealth >= caster.maxHealth)
        {
            caster.health = caster.maxHealth;
        }
        else
        {
            caster.health = newHealth;
        }
    }

    void f_SummonDog([[maybe_unused]] Hero &caster, [[maybe_unused]] Hero &target, Combat &combat)
    {
        auto doggo = Files::loadHeroFromConfig("assets/characters/dog/dog.txt");

        doggo.actionPoints = 2;

        auto isSameId = [&](const auto &h) { return h.uniqueId == doggo.uniqueId; };

        // check if there already is a doggo
        // if yes, then heal it and remove status effects
        auto foundDogo = std::find_if(begin(combat.turnQueue), end(combat.turnQueue), isSameId);
        if (foundDogo != std::end(combat.turnQueue))
        {
            (*foundDogo).health = doggo.maxHealth;
            (*foundDogo).statusEffects = {};
            (*foundDogo).actionPoints = doggo.actionPoints;
            return;
        }

        // check if doggo in spawn queue
        if (std::any_of(combat.spawnQueue.cbegin(), combat.spawnQueue.cend(), isSameId))
        {
            return;
        }

        combat.spawnQueue.push_back(doggo);
    }

    void f_HailStorm(const Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
    {
        auto damage = 5 + caster.level * 10 + Dice::randomSelection(0, 10);  // NOLINT
        Characters::takeDamage(target, damage);
    }

    void f_ReviveDead(const Hero &caster, [[maybe_unused]] Hero &target, Combat &combat)
    {
        if (combat.dead.size() == 0)
        {
            return;
        }

        const auto index = Dice::randomSelection(0, combat.dead.size() - 1);
        auto revived = combat.dead[index];
        revived.health = revived.maxHealth;
        revived.controller = caster.controller;
        revived.actionPoints = 3;
        combat.spawnQueue.push_back(revived);

        combat.dead.erase(combat.dead.begin() + index);
    }

}  // namespace Abilities