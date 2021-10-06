#include "abilities.hpp"

#include "entities.hpp"
#include "utils.hpp"
#include "characters.hpp"
#include "inventory_manager.hpp"
#include "files.hpp"
#include "dice.hpp"

namespace Abilities
{
    std::unordered_map<std::string, Ability> loadedAbilities;

    std::optional<Ability> getAbility(const std::string &id)
    {
        if (id.empty())
        {
            return {};
        }
        return loadedAbilities.at(id);
    }

    void init()
    {
        const auto abilitiesFromFile = Files::loadAbilities("assets/abilities/abilities.txt");
        for (auto ab : abilitiesFromFile)
        {
            loadedAbilities[ab.id] = ab;
        }
    }

    void learnAbility(Hero &hero, const std::string &id)
    {
        if (getAbility(id).has_value())
        {
            hero.abilities.push_back(id);
        }
    }

    void executeAbility(const std::string &id, Hero &caster, Hero &target, Combat &combat)
    {
        if (getAbility(id).has_value())
        {
            const auto ability = getAbility(id).value();

            if (ability.type == AbilityType::StatusEffect || ability.type == AbilityType::AoE_Status)
            {
                auto se = g_StatusEffects.at(ability.mapping);

                // prevent applying twice
                for (auto &seApplied : target.statusEffects)
                {
                    if (se.name == seApplied.name)
                    {
                        seApplied.turnsLeft = se.turnsLeft;
                        return;
                    }
                }

                target.statusEffects.push_back(se);
            }
            else
            {
                auto f = mappedFunctions.at(ability.mapping);
                f(caster, target, combat);
            }
        }
    }

    void f_Maul(Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
    {
        auto damage = 5 + caster.level * 10;
        Characters::takeDamage(target, damage);
    }

    void f_FirstAid(Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
    {
        if (target.controller == Controller::Player || target.controller == Controller::AI_Friendly)
        {
            // level * 5
            uint32_t newHealth = 20 + target.health + caster.level * 5;
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
        uint32_t healthStolen = 10 + caster.level * 5;

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

    void f_SummonDog(Hero &caster, [[maybe_unused]] Hero &target, Combat &combat)
    {
        auto doggo = Files::loadHeroFromConfig("assets/characters/dog/dog.txt");

        doggo.actionPoints = 2;

        // check if there already is a doggo
        // if yes, then heal it and remove status effects
        for (auto &h : combat.turnQueue)
        {
            if (h.uniqueId == doggo.uniqueId)
            {
                h.health = doggo.maxHealth;
                h.statusEffects = {};
                h.actionPoints = doggo.actionPoints;
                return;
            }
        }

        // insert behind caster
        std::vector<Hero> newQueue;
        uint32_t newHeroPos = 0;
        for (const auto &h : combat.turnQueue)
        {
            newQueue.push_back(h);
            if (h.uniqueId == caster.uniqueId)
            {
                newHeroPos = newQueue.size() - 1;
                newQueue.push_back(doggo);
            }
        }

        combat.turnQueue = newQueue;
        combat.currentHero = newHeroPos;
    }

    void f_HailStorm(Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
    {
        auto damage = 5 + caster.level * 10 + Dice::randomSelection(0, 10);
        Characters::takeDamage(target, damage);
    }

}