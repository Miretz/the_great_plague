#include "abilities.hpp"

#include "utils.hpp"
#include "status_effects.hpp"

#include <iostream>

namespace Abilities
{

    Ability pickStartingAbility()
    {
        clearScreen();

        auto prompt = []()
        {
            printBorder(130);
            std::cout << "|Pick a starting ability:\n";
            printBorder(130);
            std::cout << "\n";
        };

        std::vector<std::string> menu;
        for (auto sa : startingAbilities)
        {
            menu.push_back(sa.name + " - " + sa.description);
        }

        uint32_t selection = pickOptionFromList(prompt, menu);
        return startingAbilities[selection];
    }

    void learnAbility(Hero &hero, Ability ability)
    {
        hero.abilities.push_back(ability);
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

    void a_Evasion(Hero &caster, Hero &target)
    {
        if (caster.name == target.name)
        {
            caster.statusEffects.push_back(StatusEffects::Evasion);
        }
    }

    void a_Knockdown(Hero &caster, Hero &target)
    {
        if (caster.name != target.name && target.controller == Controller::AI_Enemy)
        {
            target.statusEffects.push_back(StatusEffects::Knockdown);
        }
    }

    void a_VampiricBite(Hero &caster, Hero &target)
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

    void a_MagicShield(Hero &caster, Hero &target)
    {
        if (target.controller == Controller::Player || target.controller == Controller::AI_Friendly)
        {
            caster.statusEffects.push_back(StatusEffects::MagicShield);
        }
    }

    void a_SummonImp(Hero &caster, Hero &target)
    {
        //TODO: implement
        std::cout << "\nExecuted ability: Summon Familiar by " << caster.name << " on " << target.name << "\n";
        exit(-1);
    }

    void a_RaiseCorpse(Hero &caster, Hero &target)
    {
        //TODO: implement
        std::cout << "\nExecuted ability: Raise Corpse by " << caster.name << " on " << target.name << "\n";
        exit(-1);
    }

    void a_Camouflage(Hero &caster, Hero &target)
    {
        //TODO: implement
        std::cout << "\nExecuted ability: Summon Familiar by " << caster.name << " on " << target.name << "\n";
        exit(-1);
    }
    void a_ArcaneBlast(Hero &caster, Hero &target)
    {
        //TODO: implement
        std::cout << "\nExecuted ability: Summon Familiar by " << caster.name << " on " << target.name << "\n";
        exit(-1);
    }
}