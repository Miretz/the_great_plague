#include "abilities.hpp"

#include "entities.hpp"
#include "utils.hpp"

namespace Abilities
{

    void learnAbility(Hero &hero, uint32_t abilityId)
    {
        if (abilityId != 0)
        {
            hero.abilities.push_back(abilityId);
        }
    }

    void a_FirstAid(Hero &caster, Hero &target)
    {
        if (target.controller == Controller::Player || target.controller == Controller::AI_Friendly)
        {
            // level * 5
            uint32_t newHealth = target.health + caster.level * 5;
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

    void a_LifeDrain(Hero &caster, Hero &target)
    {
        // take level*5 from enemy health
        uint32_t healthStolen = caster.level * 5;

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

}