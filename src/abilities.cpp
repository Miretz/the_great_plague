#include "abilities.hpp"

#include "entities.hpp"
#include "utils.hpp"
#include "characters.hpp"
#include "inventory_manager.hpp"
#include "files.hpp"

namespace Abilities
{

    void learnAbility(Hero &hero, uint32_t abilityId)
    {
        if (abilityId != 0)
        {
            hero.abilities.push_back(abilityId);
        }
    }

    void a_Maul(Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
    {
        auto damage = 5 + caster.level * 10;
        Characters::takeDamage(target, damage);
    }

    void a_FirstAid(Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
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

    void a_LifeDrain(Hero &caster, Hero &target, [[maybe_unused]] Combat &combat)
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

    void a_SummonDog(Hero &caster, [[maybe_unused]] Hero &target, Combat &combat)
    {
        auto doggo = Files::loadHeroFromConfig("assets/characters/dog/dog.txt");

        // check if there already is a doggo
        for (auto h : combat.turnQueue)
        {
            if (h.name == doggo.name)
            {
                return;
            }
        }

        // insert behind caster
        std::vector<Hero> newQueue;
        for (auto h : combat.turnQueue)
        {
            newQueue.push_back(h);
            if (h.name == caster.name)
            {
                newQueue.push_back(doggo);
            }
        }

        combat.turnQueue = newQueue;
    }

}